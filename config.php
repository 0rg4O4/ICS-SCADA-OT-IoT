<?php
// Configuration file for CWE-434 Baxter Demo
// WARNING: This is intentionally vulnerable - DO NOT USE IN PRODUCTION!

// Display errors for educational purposes
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

// Define paths
define('BASE_PATH', dirname(__DIR__) . DIRECTORY_SEPARATOR);
define('UPLOAD_DIR', BASE_PATH . 'uploads' . DIRECTORY_SEPARATOR);
define('ALLOWED_EXTENSIONS', ['jpg', 'jpeg', 'png', 'gif', 'bmp']); // Not used in vulnerable version!

// Baxter-specific configuration (for educational context)
define('DEVICE_MODEL', 'Sigma Spectrum Infusion Pump v6.05');
define('DEVICE_SOFTWARE', '35700BAX');
define('WBM_VERSION', '16');

// Vulnerable settings (demonstrating CWE-434)
define('VULNERABLE_MODE', true); // Set to false to see secure version

// Create upload directory if it doesn't exist
if (!file_exists(UPLOAD_DIR)) {
    mkdir(UPLOAD_DIR, 0777, true);
}

// Log function for educational tracking
function log_vulnerability($message) {
    $log_file = BASE_PATH . 'vulnerability_log.txt';
    $timestamp = date('Y-m-d H:i:s');
    $log_entry = "[$timestamp] $message\n";
    file_put_contents($log_file, $log_entry, FILE_APPEND);
}
?>