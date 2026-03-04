import java.sql.*;

public class BankManager{
    
    // Simulated method that exposes query in error message (CWE-200)
    public void getUserBankAccount(String username, String accountNumber) {
        String query = null;
        
        try {
            // Simulate database query (will fail since no real DB)
            if (username.equals("john")) {
                query = "SELECT * FROM accounts WHERE owner = '" + username 
                        + "' AND accountID = " + accountNumber;
                
                // This will throw SQLException (no driver, no DB)
                Connection conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/bank");
                Statement stmt = conn.createStatement();
                ResultSet rs = stmt.executeQuery(query);
            }
            
        } catch (SQLException ex) {
            // VULNERABLE: Exposes full query with sensitive schema information
            String logMessage = "Unable to retrieve account information from database,\nquery: " + query;
            System.out.println("ERROR: " + logMessage);
            System.out.println("Exception: " + ex.getMessage());
        }
    }
    
    public static void main(String[] args) {
        BankManager bm = new BankManager();
        
        // Simulate attack - trigger error with special characters
        System.out.println("=== CWE-200 Information Exposure Demo ===\n");
        
        // Test case that exposes database schema
        bm.getUserBankAccount("john", "12345' OR '1'='1");
        
        System.out.println("\n[!] The error message above reveals:");
        System.out.println("    - Table name: 'accounts'");
        System.out.println("    - Column names: 'owner', 'accountID'");
        System.out.println("    - Query structure/pattern");
    }
}