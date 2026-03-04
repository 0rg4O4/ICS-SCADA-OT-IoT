<?php
require_once 'includes/config.php';

// Log the attempt
log_vulnerability("File upload attempt detected");

// Check if file was uploaded
if (!isset($_FILES['file'])) {
    die("No file uploaded.");
}

$file = $_FILES['file'];

// Display upload information (educational)
echo "<!DOCTYPE html><html><head><title>Upload Result</title>";
echo "<link rel='stylesheet' href='css/style.css'>";
echo "<style>
    body { font-family: Arial; padding: 20px; background: #f4f4f4; }
    .result { max-width: 800px; margin: 0 auto; background: white; padding: 20px; border-radius: 5px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }
    .vulnerability { background: #ffebee; color: #c62828; padding: 15px; border-radius: 3px; margin: 20px 0; border-left: 4px solid #c62828; }
    .file-info { background: #e3f2fd; padding: 10px; border-radius: 3px; }
    .warning { background: #fff3e0; color: #ef6c00; padding: 10px; border-radius: 3px; }
    .cmd-exec { background: #f3e5f5; padding: 15px; border-radius: 3px; margin-top: 20px; }
    pre { background: #f5f5f5; padding: 10px; border-radius: 3px; overflow-x: auto; }
</style></head><body>";
echo "<div class='result'>";
echo "<h2>📤 File Upload Result</h2>";

// VULNERABILITY: No file type validation (CWE-434 demonstration)
$filename = basename($file['name']);
$target_path = UPLOAD_DIR . $filename;

echo "<div class='vulnerability'>";
echo "<strong>⚠️ CWE-434 VULNERABILITY DEMONSTRATION</strong><br>";
echo "This code intentionally lacks file validation to show how unrestricted file upload works.";
echo "</div>";

echo "<div class='file-info'>";
echo "<h3>📋 Upload Information:</h3>";
echo "<strong>Original filename:</strong> " . htmlspecialchars($filename) . "<br>";
echo "<strong>File size:</strong> " . round($file['size'] / 1024, 2) . " KB<br>";
echo "<strong>File type (declared):</strong> " . htmlspecialchars($file['type']) . "<br>";
echo "<strong>Temporary location:</strong> " . htmlspecialchars($file['tmp_name']) . "<br>";
echo "<strong>Upload timestamp:</strong> " . date('Y-m-d H:i:s') . "<br>";
echo "</div>";

// Check for potential malicious intent
$extension = strtolower(pathinfo($filename, PATHINFO_EXTENSION));
if ($extension == 'php') {
    echo "<div class='warning'>";
    echo "<strong>⚠️ WARNING: PHP file detected!</strong><br>";
    echo "This file contains executable code. In a vulnerable system (like older Baxter pumps with FTP access), ";
    echo "this could be used to execute arbitrary commands on the device [citation:2].";
    echo "</div>";
}

// Move the uploaded file (VULNERABLE: no checks!)
if (move_uploaded_file($file['tmp_name'], $target_path)) {
    echo "<p style='color: green;'>✅ File successfully uploaded to device storage.</p>";
    
    // Log successful upload
    log_vulnerability("File uploaded: $filename (Size: {$file['size']} bytes)");
    
    // Display the file URL
    $file_url = 'uploads/' . $filename;
    echo "<p>📁 Location: <code>" . htmlspecialchars($target_path) . "</code></p>";
    echo "<p>🔗 Access at: <a href='$file_url' target='_blank'>$file_url</a></p>";
    
    // If it's a PHP file, show execution demonstration
    if ($extension == 'php') {
        echo "<div class='cmd-exec'>";
        echo "<h3>💻 Remote Code Execution Demo</h3>";
        echo "<p>Since this is a PHP file and the server executes PHP, you can now:</p>";
        echo "<ol>";
        echo "<li>Access the file directly: <a href='$file_url' target='_blank'>$file_url</a></li>";
        echo "<li>Execute commands by adding ?cmd=your_command to the URL</li>";
        echo "</ol>";
        echo "<p><strong>Example:</strong> <code>{$file_url}?cmd=dir</code> (Windows) or <code>{$file_url}?cmd=ls</code> (Linux)</p>";
        echo "<p><em>This simulates how an attacker could gain remote access to a medical device, similar to how unauthenticated SSH access (CVE-2014-5432) could be exploited [citation:4]</em></p>";
        echo "</div>";
    }
    
    // Baxter-specific context
    echo "<div style='margin-top: 20px; background: #e8f5e8; padding: 10px; border-radius: 3px;'>";
    echo "<h4>🔬 Baxter Medical Device Context:</h4>";
    echo "<p>In actual Baxter Sigma Spectrum pumps, similar vulnerabilities included:</p>";
    echo "<ul>";
    echo "<li><strong>CVE-2014-5432</strong>: Unauthenticated SSH access on port 22 [citation:4]</li>";
    echo "<li><strong>CVE-2014-5434</strong>: Hard-coded FTP credentials [citation:2][citation:6]</li>";
    echo "<li><strong>CVE-2014-5433</strong>: Wireless credentials in cleartext [citation:5]</li>";
    echo "</ul>";
    echo "<p>Combined with unrestricted file upload, an attacker could upload malicious firmware or configuration files, similar to this demonstration.</p>";
    echo "</div>";
    
} else {
    echo "<p style='color: red;'>❌ Upload failed. Check permissions.</p>";
    log_vulnerability("Upload failed for: $filename");
}

echo "<p style='margin-top: 20px;'><a href='index.php'>← Back to upload form</a></p>";
echo "</div></body></html>";
?>