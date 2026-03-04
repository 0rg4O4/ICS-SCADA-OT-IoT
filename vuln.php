<?php
// Simulate database connection
$Mysql_config_location = "C:\\xampp\\htdocs\\cwe434_baxter_demo\\config\\database.ini";

try {
    // Simulate connection failure
    if (rand(0, 1)) { // Randomly fail for demonstration
        throw new Exception("Can't connect to MySQL server");
    }
    echo "Database connected successfully\n";
} catch (Exception $e) {
    // VULNERABLE: Exposes config file location (CWE-200)
    echo "Caught exception: " . $e->getMessage() . "\n";
    echo "Check credentials in config file at: " . $Mysql_config_location . "\n";
}
?>