CWE-434 DEMONSTRATION FOR BAXTER SIGMA INFUSION PUMPS
======================================================

PURPOSE:
This demonstration shows how CWE-434 (Unrestricted File Upload) could be exploited
in medical device contexts, specifically referencing vulnerabilities found in
Baxter Sigma Spectrum Infusion Pumps.

RELATED BAXTER VULNERABILITIES:
- CVE-2014-5431: Hard-coded password for biomedical information [citation:1][citation:10]
- CVE-2014-5432: Unauthenticated SSH access on port 22 [citation:4][citation:5]
- CVE-2014-5433: Wireless credentials in cleartext [citation:1][citation:5]
- CVE-2014-5434: Hard-coded FTP credentials [citation:2][citation:6][citation:8]
- CVE-2020-12039: Hardcoded passwords for biomedical menus [citation:1][citation:7]

HOW TO USE:
1. Place this folder in C:\xampp\htdocs\cwe434_baxter_demo\
2. Start Apache from XAMPP Control Panel
3. Access: http://localhost/cwe434_baxter_demo/
4. Upload malicious.php through the form
5. Access the uploaded file at: http://localhost/cwe434_baxter_demo/uploads/malicious.php?cmd=dir

DEMONSTRATION STEPS:
1. Show the vulnerable upload form (simulating pump configuration interface)
2. Upload malicious.php (simulating attacker uploading malicious firmware)
3. Execute commands on the server (simulating remote code execution)
4. Reference actual Baxter CVEs to show real-world context

SAFETY NOTES:
- This is for EDUCATIONAL PURPOSES ONLY
- Run only on isolated local systems
- Do not expose to networks
- Delete after learning

REFERENCES:
[1] CVE database entries for Baxter devices [citation:1]
[2] ICS-CERT Advisory ICSA-15-181-01 [citation:2]
[3] FDA recall notice for Sigma Spectrum pumps [citation:9]
[4] Rapid7 disclosure of Baxter vulnerabilities [citation:3]