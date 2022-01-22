import java.sql.*;
public class Demo {
    static final String JDBC_DRIVER = "com.mysql.cj.jdbc.Driver";
    static final String DB_URL = "jdbc:mysql://localhost:3306/comapanydb?usessl=False";

    static final String user = "root";
    static final String password = "";
    public static void main(String[] args){
        Connection conn = null;
        Statement stmt = null;
        try {
            Class.forName(JDBC_DRIVER);
            System.out.println("Connecting to Database");
            System.out.println("Creating Statement");
            conn = DriverManager.getConnection(DB_URL,user,password);
            stmt = conn.createStatement();
            String sql;
            sql = "Select fname, lname from employee";
            ResultSet rs = stmt.executeQuery(sql);
            while (rs.next()){
                String fname = rs.getString("fname");
                String lname = rs.getString("lname");

                System.out.print("fname:"+fname+" ");
                System.out.println("lname:"+lname);
            }
            rs.close();
            stmt.close();
            conn.close();
        } catch (Exception throwable) {
            throwable.printStackTrace();
        }finally {
            try {
                if(stmt!=null){
                    stmt.close();
                }
            }catch (SQLException se2){
                se2.printStackTrace();
            }
            try {
                if(conn!=null){
                    conn.close();
                }
            }catch (SQLException se){
                se.printStackTrace();
            }
            System.out.println("End of code");
        }
    }


}