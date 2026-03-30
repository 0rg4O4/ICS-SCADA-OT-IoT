import pandas as pd
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
