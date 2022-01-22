import java.util.*;
import java.sql.*;

public class MainProj {

	static final String JDBC_DRIVER = "com.mysql.jdbc.Driver"; 
	static final String DB_URL = "jdbc:mysql://localhost/bookstoredb?useSSL=false";
	static final String USER = "root";
	static final String PASS = "Dblab21@";
	static Scanner sc = new Scanner(System.in);

	public static Connection conn = null;
	public static Statement stmt = null;






	// The main function
	public static void main(String[] args) {

		establishConn();
		Menu();
		closeConn();

	}






	// The menu driving function, which calls the functions according to the options chosen
	public static void Menu() {
		while(true) {
			System.out.println("1. Add book");
			System.out.println("2. Add customer");
			System.out.println("3. Search all books");
			System.out.println("4. Search available books");
			System.out.println("5. Search customer");
			System.out.println("6. Lend book");
			System.out.println("7. Return book");
			System.out.println("8. Search Lend Records");
			System.out.println("9. Update book details");
			System.out.println("10. Update Customer details");
			System.out.println("11. Delete Book");
			System.out.println("12. Delete Customer");
			System.out.println("0. Quit");


			int ch = Integer.parseInt(sc.nextLine());
			if(ch == 0) break;

			if(ch == 1) add_book();

			if(ch == 2) add_customer();

			if(ch == 3) search_book(0);

			if(ch == 4) search_book(1);

			if(ch == 5) search_customer();

			if(ch == 6) lend_book();

			if(ch == 7) return_book();

			if(ch == 8) search_lendRecords();

			if(ch == 9) update_book_details();

			if(ch == 10) update_customer_details();

			if(ch == 11) delete_book();

			if(ch == 12) delete_customer();


			System.out.println("");
		}
	}









	public static void establishConn() {
		try{
            //STEP 2a: Register JDBC driver
            Class.forName(JDBC_DRIVER);
            //STEP 2b: Open a connection
            System.out.println("Connecting to database...");
            conn = DriverManager.getConnection(DB_URL,USER,PASS);
        }

        catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}

	public static void closeConn() {
		try{
			if(conn!=null)
				conn.close();
		}
		catch(SQLException se){
			se.printStackTrace();
		}     
	}






	// Function to add a new book
	public static void add_book() {

		try {
			// System.out.println("Creating statement...");
			stmt = conn.createStatement();

			System.out.println("Give the book name : ");
			String name = sc.nextLine();

			System.out.println("Give the book author : ");
			String author = sc.nextLine();

			System.out.println("Give the book genre : ");
			String genre = sc.nextLine();

			String sql;
			sql = "insert into book(bname, author, genre) values(\"" + name + "\", \"" + author + "\", \"" + genre + "\");";
			stmt.executeUpdate(sql);

			stmt.close();
		}

		catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}


	// Function to add a new customer
	public static void add_customer() {

		try {
			// System.out.println("Creating statement...");
			stmt = conn.createStatement();

			System.out.println("Give the customer name : ");
			String name = sc.nextLine();

			System.out.println("Give the customer aadhar number : ");
			String aadhar = sc.nextLine();

			System.out.println("Give the customer address: ");
			String address = sc.nextLine();

			System.out.println("Give the customer sex: (M for male, F for female)");
			String sex = sc.nextLine().toUpperCase();

			String sql;
			sql = "insert into customer(cname, aadhar_num, address, sex) values(\"" + name + "\", \"" + aadhar + "\", \"" + address + "\", \"" + sex + "\");";
			stmt.executeUpdate(sql);

			stmt.close();
		}

		catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}

	// Function to lend someone some book
	public static void lend_book() {

		try {
			// System.out.println("Creating statement...");
			stmt = conn.createStatement();

			System.out.println("Give the customer aadhar number : ");
			String aadhar = sc.nextLine();

			System.out.println("Give the book id: ");
			int bid = Integer.parseInt(sc.nextLine());

			String sql;
			sql = "insert into lend_info(book_id, customer_id) values(\"" + bid + "\", " + aadhar + ");";
			stmt.executeUpdate(sql);

			stmt.close();
		}

		catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}



	// Function to delete a book
	public static void return_book() {
		try {
			// System.out.println("Creating statement...");
			stmt = conn.createStatement();

			System.out.println("Id for the book to be returned : ");
			int bid = Integer.parseInt(sc.nextLine());

			System.out.println("Aadhar of the returnee : ");
			String aadh = sc.nextLine();

			String sql;
			sql = "delete from lend_info where book_id = " + Integer.toString(bid) + " and customer_id = " + aadh + ";";
			stmt.executeUpdate(sql);

			stmt.close();
		}

		catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}


	public static void search_lendRecords() {


		try {
			// System.out.println("Creating statement...");
			stmt = conn.createStatement();
			ResultSet rs;

			System.out.println("Give the book id : (Leave empty to display all records)");
			int all = 0;
			String inp = sc.nextLine();

			if(inp.length() == 0) {
				all = 1;
			}

			String sql;

			if(all == 0) {
				int id = Integer.parseInt(sc.nextLine());
				sql = "select lend_info.book_id as bid, customer_id, bname, cname from lend_info, customer, book where customer_id = aadhar_num and book.book_id = lend_info.book_id and lend_info.book_id = " + Integer.toString(id) + ";";
			}
			else {
				sql = "select lend_info.book_id as bid, customer_id, bname, cname from lend_info, customer, book where customer_id = aadhar_num and book.book_id = lend_info.book_id;";
			}

			rs = stmt.executeQuery(sql);
	        while(rs.next()){
	            int book_id  = rs.getInt("bid");
	            String customer_id = rs.getString("customer_id");
	            String bname = rs.getString("bname");
	            String cname = rs.getString("cname");

	            //Display values
	            System.out.print("Book Id: " + book_id);
	            System.out.print(", Customer Id: " + customer_id);
	            System.out.print(", Book Name: " + bname);
	            System.out.println(", Customer Name: " + cname);

	        }

			rs.close();
			stmt.close();

		}

		catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}

	// Search books
	public static void search_book(int avail) {

		try {

			String extracond = "";
			if(avail == 1) {
				extracond = " and book_id not in (select book_id from lend_info)";
			}

			// System.out.println("Creating statement...");
			stmt = conn.createStatement();
			ResultSet rs;



			System.out.println("1. Search By name");
			System.out.println("2. Search By id");
			int nm = Integer.parseInt(sc.nextLine());

			if(nm == 1) {
				System.out.println("Give the book name : (Leave empty to display all books)");
				String name = sc.nextLine();

				String sql;
				sql = "select * from book where bname like \"%" + name + "%\"" + extracond + ";";

				rs = stmt.executeQuery(sql);
	            while(rs.next()){
	                
	                int book_id  = rs.getInt("book_id");
	                String bname = rs.getString("bname");
	                String author = rs.getString("author");
	                String genre = rs.getString("genre");

	                //Display values
	                System.out.print("Id: " + book_id);
	                System.out.print(", Name: " + bname);
	                System.out.print(", Author: " + author);
	                System.out.println(", Genre: " + genre);

	            }
			}
			else {
				System.out.println("Give the book id : ");
				int id = Integer.parseInt(sc.nextLine());

				String sql;
				sql = "select * from book where book_id = " + Integer.toString(id) + extracond + ";";

				rs = stmt.executeQuery(sql);
	            while(rs.next()){
	                
	                int book_id  = rs.getInt("book_id");
	                String bname = rs.getString("bname");
	                String author = rs.getString("author");
	                String genre = rs.getString("genre");

	                //Display values
	                System.out.print("Id: " + book_id);
	                System.out.print(", Name: " + bname);
	                System.out.print(", Author: " + author);
	                System.out.println(", Genre: " + genre);

	            }

			}

			rs.close();
			stmt.close();

		}

		catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}


	// Search customers
	public static void search_customer() {

		try {
			// System.out.println("Creating statement...");
			stmt = conn.createStatement();
			ResultSet rs;

			System.out.println("1. Search By name");
			System.out.println("2. Search By aadhar id");
			int nm = Integer.parseInt(sc.nextLine());

			if(nm == 1) {
				System.out.println("Give the customer name : (Leave empty to display all customers)");
				String name = sc.nextLine();

				String sql;
				sql = "select * from customer where cname like \"%" + name + "%\";";

				rs = stmt.executeQuery(sql);
	            while(rs.next()){
	                
	                String cname  = rs.getString("cname");
	                String aadhar_num = rs.getString("aadhar_num");
	                String address = rs.getString("address");
	                String sex = rs.getString("sex");

	                //Display values
	                System.out.print("Name: " + cname);
	                System.out.print(", Aadhar: " + aadhar_num);
	                System.out.print(", Address: " + address);
	                System.out.println(", Sex: " + sex);

	            }
			}
			else {
				System.out.println("Give the customer name : ");
				String aadh = sc.nextLine();

				String sql;
				sql = "select * from customer where aadhar_num like \"%" + aadh + "%\";";

				rs = stmt.executeQuery(sql);
	            while(rs.next()){
	                
	                String cname  = rs.getString("cname");
	                String aadhar_num = rs.getString("aadhar_num");
	                String address = rs.getString("address");
	                String sex = rs.getString("sex");

	                //Display values
	                System.out.print("Name: " + cname);
	                System.out.print(", Aadhar: " + aadhar_num);
	                System.out.print(", Address: " + address);
	                System.out.println(", Sex: " + sex);

	            }
			}

			rs.close();
			stmt.close();

		}

		catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}






	// Function to delete a book
	public static void delete_book() {
		try {
			// System.out.println("Creating statement...");
			stmt = conn.createStatement();

			System.out.println("Id for the book to be deleted : ");
			int bid = Integer.parseInt(sc.nextLine());

			String sql;
			sql = "delete from book where book_id = " + Integer.toString(bid) + ";";
			stmt.executeUpdate(sql);

			stmt.close();
		}

		catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}

	// Fucntino to delete a customer
	public static void delete_customer() {
		try {
			// System.out.println("Creating statement...");
			stmt = conn.createStatement();

			System.out.println("Aadhar for the customer to be deleted : ");
			String aadh = sc.nextLine();

			String sql;
			sql = "delete from customer where aadhar_num = " + aadh + ";";
			stmt.executeUpdate(sql);

			stmt.close();
		}

		catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}








	// Function to update book details
	public static void update_book_details() {

		try {
			// System.out.println("Creating statement...");
			stmt = conn.createStatement();

			System.out.println("Id for the book to be updated : ");
			int bid = Integer.parseInt(sc.nextLine());

			String sql;
			sql = "select * from book where book_id = " + Integer.toString(bid) + ";";




			int book_id;
			String bname = "", author = "", genre = "";

			ResultSet rs = stmt.executeQuery(sql);
            while(rs.next()){
                
                book_id  = rs.getInt("book_id");
                bname = rs.getString("bname");
                author = rs.getString("author");
                genre = rs.getString("genre");
            }
            rs.close();





            String opt;

  			System.out.println("Old Name : " + bname);
            System.out.println("Do you want to change name? y/n");
            opt = sc.nextLine();
            if(opt.equals("y")) {
            	System.out.println("Give the new book name : ");
            	bname = sc.nextLine();

            }

            System.out.println("Old Author : " + author);
            System.out.println("Do you want to change author? y/n");
            opt = sc.nextLine();
            if(opt.equals("y")) {
            	System.out.println("Give the new book author : ");
            	author = sc.nextLine();

            }

            System.out.println("Old Genre : " + genre);
            System.out.println("Do you want to change genre? y/n");
            opt = sc.nextLine();
            if(opt.equals("y")) {
            	System.out.println("Give the new book genre : ");
            	genre = sc.nextLine();

            }


            


			sql = "update book set bname = \"" + bname + "\", author = \"" + author + "\", genre = \"" + genre + "\" where book_id = " + Integer.toString(bid) + ";";

			stmt.executeUpdate(sql);

			stmt.close();
		}

		catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}


	// Fucntion to update customer_details
	public static void update_customer_details() {
		try {
			// System.out.println("Creating statement...");
			stmt = conn.createStatement();

			System.out.println("Aadhar for the customer to be updated : ");
			String aadh = sc.nextLine();

			String sql;
			sql = "select * from customer where aadhar_num = " + aadh + ";";




			String aadhar_num = "", cname = "", address = "", sex = "";

			ResultSet rs = stmt.executeQuery(sql);
            while(rs.next()){
                
                aadhar_num  = rs.getString("aadhar_num");
                cname = rs.getString("cname");
                address = rs.getString("address");
                sex = rs.getString("sex");
            }
            rs.close();





            String opt;

  			System.out.println("Old Name : " + cname);
            System.out.println("Do you want to change name? y/n");
            opt = sc.nextLine();
            if(opt.equals("y")) {
            	System.out.println("Give the new Customer name : ");
            	cname = sc.nextLine();

            }

            System.out.println("Old Address : " + address);
            System.out.println("Do you want to change address? y/n");
            opt = sc.nextLine();
            if(opt.equals("y")) {
            	System.out.println("Give the new customer address : ");
            	address = sc.nextLine();

            }

            System.out.println("Old gender : " + sex);
            System.out.println("Do you want to change gender of the customer? y/n");
            opt = sc.nextLine();
            if(opt.equals("y")) {
            	System.out.println("Give the new customer gender : ");
            	sex = sc.nextLine();

            }




			sql = "update customer set cname = \"" + cname + "\", address = \"" + address + "\", sex = \"" + sex + "\" where aadhar_num = " + aadhar_num + ";";
			stmt.executeUpdate(sql);

			stmt.close();
		}

		catch(SQLException se){         //Handle errors for JDBC
            se.printStackTrace();
        }

        catch(Exception e){            //Handle errors for Class.forName
          e.printStackTrace();
       	}
	}


}


