<?php
// Simulate getting current username
$userName = get_current_user(); // Returns the username of the script owner

// VULNERABLE: Exposing all processes for the current user (CWE-497)
echo "=== Processes for user: $userName ===\n\n";

// Windows equivalent of 'ps aux' is 'tasklist'
// This shows ALL processes, including command-line arguments
$command = 'tasklist /V /FO LIST';
system($command);

// Additional exposure - show even more details
echo "\n=== Additional Process Details ===\n";
$detailed = 'wmic process get processid,commandline,executablepath /FORMAT:LIST';
system($detailed);
?>