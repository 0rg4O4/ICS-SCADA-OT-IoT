public class SimpleExposure {
    public static void main(String[] args) {
        // VULNERABLE: Password comes from command line
        String password = System.getProperty("password");
        
        if (password == null) {
            System.out.println("No password provided");
        } else {
            System.out.println("Password loaded successfully");
            // In real code, you'd use the password here
        }
        
        // Keep program running so we can see it in tasklist
        try {
            Thread.sleep(10000); // Wait 10 seconds
        } catch (InterruptedException e) {
            // Ignore
        }
    }
}