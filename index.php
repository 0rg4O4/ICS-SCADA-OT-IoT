<?php
require_once 'includes/config.php';
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Baxter Sigma Pump - CWE-434 Demo</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            margin: 0;
            padding: 20px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
        }
        .container {
            max-width: 1000px;
            margin: 0 auto;
            background: white;
            padding: 30px;
            border-radius: 10px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
        }
        h1 {
            color: #333;
            border-bottom: 2px solid #667eea;
            padding-bottom: 10px;
        }
        .warning-banner {
            background: #fff3cd;
            color: #856404;
            padding: 15px;
            border-radius: 5px;
            margin-bottom: 20px;
            border-left: 4px solid #ffc107;
        }
        .vulnerability-info {
            background: #f8d7da;
            color: #721c24;
            padding: 15px;
            border-radius: 5px;
            margin-bottom: 20px;
            border-left: 4px solid #dc3545;
        }
        .device-info {
            background: #e2e3e5;
            color: #383d41;
            padding: 10px;
            border-radius: 5px;
            margin-bottom: 20px;
        }
        .upload-form {
            background: #f8f9fa;
            padding: 20px;
            border-radius: 5px;
            margin-bottom: 20px;
        }
        .form-group {
            margin-bottom: 15px;
        }
        label {
            display: block;
            margin-bottom: 5px;
            font-weight: bold;
            color: #333;
        }
        input[type="file"] {
            width: 100%;
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 5px;
            background: white;
        }
        input[type="submit"] {
            background: #667eea;
            color: white;
            border: none;
            padding: 12px 30px;
            border-radius: 5px;
            cursor: pointer;
            font-size: 16px;
        }
        input[type="submit"]:hover {
            background: #5a67d8;
        }
        .cve-list {
            background: #f8f9fa;
            padding: 20px;
            border-radius: 5px;
        }
        .cve-item {
            margin-bottom: 15px;
            padding: 10px;
            border-left: 3px solid #667eea;
            background: white;
        }
        .cve-item.critical {
            border-left-color: #dc3545;
        }
        .uploaded-files {
            margin-top: 20px;
        }
        .file-item {
            padding: 10px;
            background: #f8f9fa;
            margin-bottom: 5px;
            border-radius: 3px;
        }
        .file-item a {
            color: #667eea;
            text-decoration: none;
        }
        .file-item a:hover {
            text-decoration: underline;
        }
        .security-toggle {
            margin: 20px 0;
            text-align: right;
        }
        .security-toggle button {
            padding: 8px 15px;
            background: #6c757d;
            color: white;
            border: none;
            border-radius: 3px;
            cursor: pointer;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Baxter Sigma Infusion Pump Security Demo</h1>
        <h3>CWE-434: Unrestricted File Upload Vulnerability</h3>
        
        <div class="warning-banner">
            <strong>⚠️ EDUCATIONAL PURPOSE ONLY</strong><br>
            This demonstration simulates vulnerabilities found in Baxter Sigma Spectrum Infusion Pumps 
            for cybersecurity education. DO NOT use in production environments.
        </div>

        <div class="vulnerability-info">
            <strong>🔴 ACTIVE VULNERABILITY: CWE-434 (Unrestricted File Upload)</strong><br>
            This server intentionally allows uploading and executing arbitrary PHP files,
            demonstrating how an attacker could compromise a medical device system.
        </div>

        <div class="device-info">
            <strong>Medical Device Simulation:</strong><br>
            Model: <?php echo DEVICE_MODEL; ?><br>
            Product Code: <?php echo DEVICE_SOFTWARE; ?><br>
            WBM Version: <?php echo WBM_VERSION; ?><br>
            <small>Based on actual vulnerabilities: CVE-2014-5431, CVE-2014-5432, CVE-2014-5433, CVE-2014-5434 [citation:1][citation:2]</small>
        </div>

        <div class="upload-form">
            <h3>📤 Device Configuration Upload</h3>
            <p><em>Simulating pump configuration and firmware update interface</em></p>
            
            <form action="upload.php" method="post" enctype="multipart/form-data">
                <div class="form-group">
                    <label for="file">Select configuration file or firmware update:</label>
                    <input type="file" name="file" id="file" required>
                </div>
                <div class="form-group">
                    <input type="submit" value="Upload to Device">
                </div>
            </form>
        </div>

        <div class="cve-list">
            <h3>📋 Related Baxter Vulnerabilities</h3>
            
            <div class="cve-item critical">
                <strong>CVE-2014-5434</strong> (CVSS 9.8 Critical)<br>
                Default account with hard-coded credentials used with FTP protocol [citation:2][citation:6][citation:8]
            </div>
            
            <div class="cve-item critical">
                <strong>CVE-2014-5432</strong> (CVSS 9.8 Critical)<br>
                SSH accessible without authentication on port 22 [citation:4][citation:5]
            </div>
            
            <div class="cve-item critical">
                <strong>CVE-2014-5433</strong> (CVSS 9.8 Critical)<br>
                Wireless credentials stored in cleartext [citation:1][citation:5]
            </div>
            
            <div class="cve-item">
                <strong>CVE-2014-5431</strong> (CVSS 6.8 Medium)<br>
                Hard-coded password for biomedical information access [citation:1][citation:10]
            </div>
            
            <div class="cve-item">
                <strong>CVE-2020-12039</strong> (CVSS 2.4 Low)<br>
                Hardcoded passwords for biomedical menus [citation:1][citation:7]
            </div>
        </div>

        <div class="uploaded-files">
            <h3>📁 Uploaded Device Files</h3>
            <?php
            $files = scandir(UPLOAD_DIR);
            foreach ($files as $file) {
                if ($file != '.' && $file != '..') {
                    $file_url = 'uploads/' . $file;
                    echo "<div class='file-item'>";
                    echo "📄 <a href='$file_url' target='_blank'>$file</a> ";
                    echo "(" . round(filesize(UPLOAD_DIR . $file) / 1024, 2) . " KB)";
                    
                    // Show if it's a PHP file (potential malicious)
                    if (pathinfo($file, PATHINFO_EXTENSION) == 'php') {
                        echo " ⚠️ <span style='color:#dc3545;'>PHP executable - potential weapon!</span>";
                    }
                    echo "</div>";
                }
            }
            ?>
        </div>

        <div class="security-toggle">
            <button onclick="window.location.href='?secure=1'">Switch to Secure Version</button>
        </div>

        <div style="margin-top: 20px; font-size: 12px; color: #666; text-align: center;">
            <p>This demonstration is based on actual vulnerabilities discovered in Baxter Sigma Spectrum Infusion Pumps.<br>
            References: ICS-CERT advisories IC SA-15-181-01, IC SA-22-165-01 [citation:2][citation:3]</p>
        </div>
    </div>
</body>
</html>