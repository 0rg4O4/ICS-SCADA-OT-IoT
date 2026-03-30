#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>

using namespace std;

// ==================== 3D VECTOR CLASS ====================
class Vector3D {
public:
    double x, y, z;
    
    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
    
    Vector3D operator+(const Vector3D& o) const { return Vector3D(x + o.x, y + o.y, z + o.z); }
    Vector3D operator-(const Vector3D& o) const { return Vector3D(x - o.x, y - o.y, z - o.z); }
    Vector3D operator*(double s) const { return Vector3D(x * s, y * s, z * s); }
    Vector3D operator/(double s) const { return Vector3D(x / s, y / s, z / s); }
    
    double dot(const Vector3D& o) const { return x * o.x + y * o.y + z * o.z; }
    double magnitude() const { return sqrt(x*x + y*y + z*z); }
    
    Vector3D normalize() const {
        double mag = magnitude();
        if (mag < 1e-10) return Vector3D(0,0,0);
        return Vector3D(x/mag, y/mag, z/mag);
    }
};

// ==================== MISSILE CLASS ====================
class Missile {
private:
    Vector3D pos, vel, dir;
    double fuelMass;
    double thrustForce;
    double dragCoeff;
    bool active;
    
public:
    Missile(Vector3D startPos) {
        pos = startPos;
        vel = Vector3D(0,0,0);
        fuelMass = 300.0;
        thrustForce = 500000.0;      // Slightly reduced to prevent overshoot
        dragCoeff = 0.04;            // Slightly increased drag
        active = true;
        dir = Vector3D(0,0,1);
    }
    
    void setDir(Vector3D newDir) { 
        dir = newDir.normalize(); 
    }
    
    void update(double dt, Vector3D targetPos) {
        if (!active) return;
        
        double mass = 500.0 + max(0.0, fuelMass);
        
        // Thrust
        double thrust = 0;
        if (fuelMass > 0) {
            fuelMass -= 35.0 * dt;
            if (fuelMass < 0) fuelMass = 0;
            thrust = thrustForce;
        }
        
        // GUIDANCE: Proportional navigation with overshoot prevention
        Vector3D toTarget = targetPos - pos;
        double distance = toTarget.magnitude();
        
        if (distance > 0.1) {
            Vector3D desiredDir = toTarget.normalize();
            
            // Calculate angle between current direction and target
            double dot = dir.dot(desiredDir);
            double angle = acos(max(-1.0, min(1.0, dot)));
            
            // Dynamic turn rate: turn harder when far away, gentler when close
            double turnRate;
            if (distance > 1000) {
                turnRate = 2.5 * dt;  // Aggressive when far
            } else if (distance > 300) {
                turnRate = 1.5 * dt;  // Moderate when medium
            } else {
                turnRate = 0.8 * dt;  // Gentle when close (prevents overshoot)
            }
            
            // Apply turning
            dir = (dir * (1.0 - turnRate) + desiredDir * turnRate).normalize();
        }
        
        // Speed limiting to prevent overshoot
        double maxSpeed = 1200.0;  // Cap speed at Mach 3.5
        double speed = vel.magnitude();
        
        // Drag
        double rho = 1.225 * exp(-pos.z / 8500.0);
        Vector3D drag(0,0,0);
        if (speed > 0.1) {
            double dragMag = 0.5 * rho * speed * speed * dragCoeff / mass;
            drag = vel.normalize() * (-dragMag);
        }
        
        // Physics
        Vector3D gravity(0, 0, -9.81);
        Vector3D thrustAcc = dir * (thrust / mass);
        Vector3D totalAcc = gravity + thrustAcc + drag;
        
        vel = vel + totalAcc * dt;
        
        // Apply speed limit
        if (vel.magnitude() > maxSpeed) {
            vel = vel.normalize() * maxSpeed;
        }
        
        pos = pos + vel * dt;
        
        // Check if crashed
        if (pos.z < -1) active = false;
    }
    
    Vector3D getPos() const { return pos; }
    Vector3D getVel() const { return vel; }
    bool isActive() const { return active; }
    double getDistanceTo(const Vector3D& target) const { return (target - pos).magnitude(); }
};

// ==================== AUTONOMOUS TARGET ====================
class AutonomousTarget {
private:
    Vector3D pos, vel;
    mt19937 rng;
    uniform_real_distribution<double> posDist;
    uniform_real_distribution<double> velDist;
    double lastChangeTime;
    
public:
    AutonomousTarget() : rng(chrono::steady_clock::now().time_since_epoch().count()) {
        posDist = uniform_real_distribution<double>(-3000, 3000);
        velDist = uniform_real_distribution<double>(-180, 180);
        randomize();
        lastChangeTime = 0;
    }
    
    void randomize() {
        pos = Vector3D(
            posDist(rng),
            posDist(rng),
            abs(posDist(rng)) + 2000  // Start higher: 2000-5000m
        );
        vel = Vector3D(
            velDist(rng),
            velDist(rng),
            velDist(rng) * 0.15
        );
    }
    
    void update(double dt, double currentTime) {
        // Change behavior every 5-8 seconds
        if (currentTime - lastChangeTime > 5.0 + (rand() % 4)) {
            if (rand() % 100 < 35) {
                vel = Vector3D(
                    velDist(rng),
                    velDist(rng),
                    velDist(rng) * 0.15
                );
            }
            lastChangeTime = currentTime;
        }
        
        pos = pos + vel * dt;
        
        // Softer bounds
        if (abs(pos.x) > 8000) vel.x = -vel.x * 0.6;
        if (abs(pos.y) > 8000) vel.y = -vel.y * 0.6;
        if (pos.z < 1000) vel.z = abs(vel.z) + 30;
        if (pos.z > 10000) vel.z = -abs(vel.z);
    }
    
    Vector3D getPos() const { return pos; }
    Vector3D getVel() const { return vel; }
};

// ==================== VISUALIZATION DATA ====================
struct SimulationData {
    vector<double> time;
    vector<double> mx, my, mz;
    vector<double> tx, ty, tz;
    vector<double> distance;
};

// ==================== RUN SIMULATION ====================
SimulationData runSimulation(double maxTime, double dt, bool realtime = false) {
    SimulationData data;
    
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> startDist(-2000, 2000);
    
    Vector3D missileStart(
        startDist(rng),
        startDist(rng),
        0
    );
    
    AutonomousTarget target;
    Missile missile(missileStart);
    
    // Point missile toward target at launch
    Vector3D toTarget = target.getPos() - missileStart;
    if (toTarget.magnitude() > 0.1) {
        missile.setDir(toTarget.normalize());
    }
    
    cout << "\n========================================\n";
    cout << "AUTONOMOUS MISSILE SIMULATION\n";
    cout << "========================================\n";
    cout << "Missile Start: (" << (int)missileStart.x << ", " << (int)missileStart.y << ", " << (int)missileStart.z << ")\n";
    cout << "Target Start:  (" << (int)target.getPos().x << ", " << (int)target.getPos().y << ", " << (int)target.getPos().z << ")\n";
    cout << "Target Velocity: " << (int)target.getVel().magnitude() << " m/s\n";
    cout << "========================================\n\n";
    
    double t = 0;
    double minDistance = 999999;
    double hitTime = -1;
    bool destroyed = false;
    int lastPrintTime = -1;
    
    while (t <= maxTime && missile.isActive() && !destroyed) {
        target.update(dt, t);
        missile.update(dt, target.getPos());
        
        Vector3D missilePos = missile.getPos();
        Vector3D targetPos = target.getPos();
        double distance = missile.getDistanceTo(targetPos);
        
        // Store data
        if (data.time.empty() || t - data.time.back() >= 0.1) {
            data.time.push_back(t);
            data.mx.push_back(missilePos.x);
            data.my.push_back(missilePos.y);
            data.mz.push_back(missilePos.z);
            data.tx.push_back(targetPos.x);
            data.ty.push_back(targetPos.y);
            data.tz.push_back(targetPos.z);
            data.distance.push_back(distance);
        }
        
        if (distance < minDistance) {
            minDistance = distance;
            hitTime = t;
        }
        
        // Real-time output
        if (realtime && (int)t != lastPrintTime) {
            lastPrintTime = (int)t;
            cout << "[" << setw(3) << (int)t << "s] Dist: " << setw(5) << (int)distance 
                 << "m | Speed: " << setw(4) << (int)missile.getVel().magnitude() << "m/s\n";
        }
        
        // Hit detection
        if (distance < 25.0) {
            cout << "\n\n*** TARGET DESTROYED at " << fixed << setprecision(1) << t << " seconds! ***\n";
            cout << "*** Impact distance: " << distance << " meters ***\n";
            destroyed = true;
            break;
        }
        
        t += dt;
        if (realtime) this_thread::sleep_for(chrono::milliseconds((int)(dt * 100)));
    }
    
    cout << "\n\n========================================\n";
    cout << "SIMULATION RESULTS\n";
    cout << "========================================\n";
    if (destroyed) {
        cout << "✓ SUCCESS! Target eliminated!\n";
        cout << "   Time to intercept: " << fixed << setprecision(1) << hitTime << "s\n";
        cout << "   Impact distance: " << minDistance << "m\n";
    } else {
        cout << "✗ MISS! Target escaped!\n";
        cout << "   Minimum distance: " << minDistance << "m at " << hitTime << "s\n";
    }
    cout << "========================================\n";
    
    return data;
}

// ==================== PYTHON VISUALIZATION SCRIPT ====================
void generatePythonScript() {
    ofstream py("visualize.py");
    py << R"(import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

df = pd.read_csv("flight_data.csv")

distances = np.sqrt((df['mx'] - df['tx'])**2 + (df['my'] - df['ty'])**2 + (df['mz'] - df['tz'])**2)
min_idx = distances.idxmin()
min_dist = distances[min_idx]

fig = plt.figure(figsize=(15, 10))

ax1 = fig.add_subplot(221, projection='3d')
ax1.plot(df['mx'], df['my'], df['mz'], 'b-', linewidth=2, label='Missile')
ax1.plot(df['tx'], df['ty'], df['tz'], 'r--', linewidth=2, label='Target')
ax1.scatter(df['mx'].iloc[0], df['my'].iloc[0], df['mz'].iloc[0], color='blue', s=100, marker='o')
ax1.scatter(df['tx'].iloc[0], df['ty'].iloc[0], df['tz'].iloc[0], color='red', s=100, marker='o')
ax1.scatter(df['mx'].iloc[min_idx], df['my'].iloc[min_idx], df['mz'].iloc[min_idx], color='green', s=200, marker='*', label=f'Closest: {min_dist:.1f}m')
ax1.set_xlabel('X (m)'); ax1.set_ylabel('Y (m)'); ax1.set_zlabel('Z (m)')
ax1.set_title('3D Trajectories'); ax1.legend()

ax2 = fig.add_subplot(222)
ax2.plot(df['t'], distances, 'b-', linewidth=2)
ax2.axhline(y=25, color='r', linestyle='--', label='Lethal Radius (25m)')
ax2.scatter(df['t'][min_idx], min_dist, color='red', s=100)
ax2.set_xlabel('Time (s)'); ax2.set_ylabel('Distance (m)')
ax2.set_title('Miss Distance vs Time'); ax2.legend(); ax2.grid(True, alpha=0.3)

dt = df['t'].iloc[1] - df['t'].iloc[0]
missile_speed = [0]
for i in range(1, len(df)):
    dx = df['mx'].iloc[i] - df['mx'].iloc[i-1]
    dy = df['my'].iloc[i] - df['my'].iloc[i-1]
    dz = df['mz'].iloc[i] - df['mz'].iloc[i-1]
    missile_speed.append(np.sqrt(dx*dx + dy*dy + dz*dz) / dt)

ax3 = fig.add_subplot(223)
ax3.plot(df['t'], missile_speed, 'g-', linewidth=2)
ax3.set_xlabel('Time (s)'); ax3.set_ylabel('Speed (m/s)')
ax3.set_title('Missile Speed'); ax3.grid(True, alpha=0.3)

ax4 = fig.add_subplot(224)
ax4.plot(df['mx'], df['my'], 'b-', linewidth=2, label='Missile')
ax4.plot(df['tx'], df['ty'], 'r--', linewidth=2, label='Target')
ax4.set_xlabel('X (m)'); ax4.set_ylabel('Y (m)')
ax4.set_title('Top-Down View (XY Projection)')
ax4.legend(); ax4.grid(True, alpha=0.3)

plt.tight_layout()
plt.suptitle('Autonomous Missile Guidance Simulation', fontsize=14, y=1.02)
plt.show()

print(f"\nMinimum distance: {min_dist:.2f} meters")
print("✓ TARGET DESTROYED!" if min_dist < 25 else "✗ TARGET ESCAPED!")
)";
    py.close();
}

// ==================== MAIN ====================
int main() {
    cout << "╔══════════════════════════════════════════════════════════╗\n";
    cout << "║     AUTONOMOUS MISSILE GUIDANCE SIMULATION               ║\n";
    cout << "╚══════════════════════════════════════════════════════════╝\n";
    
    cout << "\nChoose mode:\n";
    cout << "  1. Fast simulation\n";
    cout << "  2. Real-time visualization\n";
    cout << "Enter choice (1 or 2): ";
    
    int choice;
    cin >> choice;
    
    SimulationData data = runSimulation(20.0, 0.05, choice == 2);
    
    ofstream csv("flight_data.csv");
    csv << "t,mx,my,mz,tx,ty,tz,distance\n";
    for (size_t i = 0; i < data.time.size(); i++) {
        csv << data.time[i] << "," << data.mx[i] << "," << data.my[i] << "," << data.mz[i] << ","
            << data.tx[i] << "," << data.ty[i] << "," << data.tz[i] << "," << data.distance[i] << "\n";
    }
    csv.close();
    
    generatePythonScript();
    
    cout << "\nData saved to 'flight_data.csv'\n";
    cout << "Run: python visualize.py\n";
    
    return 0;
}