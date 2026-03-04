#!C:\xampp\perl\bin\perl.exe
use strict;
use warnings;

# Simulated valid credentials
my $valid_username = "admin";
my $valid_password = "secret123";

# Get input from command line
my $username = $ARGV[0] || "";
my $password = $ARGV[1] || "";

# VULNERABLE: Different error messages (CWE-200)
if ($username eq $valid_username) {
    if ($password eq $valid_password) {
        print "[SUCCESS] Login Successful\n";
    } else {
        print "[FAILED] Login Failed - incorrect password\n";
    }
} else {
    print "[FAILED] Login Failed - unknown username\n";
}