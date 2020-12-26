package user;

import java.io.*;
import java.util.*;

import system.*;
import party.*;

/**
 * to help with users logging in
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019
 * Finals Week
 * kaiyihua.edu

 */
public class UserLogin {
	private HashMap<String, User> users; // map of unique email address to user objects
	private static final String USER_FILE = "src/ExistingUsers";
	private Scanner sc;
	// constructor
	public UserLogin() {
		users = new HashMap<>();
		readExistingUsersFromFile();
		sc = new Scanner(System.in);
	}
	
	// GETTER
	public HashMap<String, User> getUsers(){
		return users;
	}
	/**
	 * 
	 * @param email
	 * @return the user associated with the email or NULL if the user does not exist
	 */
	public User findUser(String email) {
		User u = users.get(email); // either a user or NULL
		return u;
	}
	
	//  create new User's 3 different versions
	public User createNewUser(String password, String email) {
		// gather the data, breaking MVC
		String name = InputHelper.readString(sc, "Please enter name:");		
		User u =  new FreeUser(password, email, name);
		
		users.put(email, u);
		this.writeSingleToFile(u);
		return u;
	}

	public User createNewUser(String email) {
		// gather the data, breaking MVC
		String name = InputHelper.readString(sc, "Please enter name:");				
		String password = InputHelper.readString(sc, "Please enter password:");		
		User u =  new FreeUser(password, email, name);

		users.put(email, u);
		this.writeSingleToFile(u);
		return u;
	}
	public User createNewUser() {
		// breaks MVC.. 
		String email = InputHelper.readString(sc, "Please enter your email:");
		return createNewUser(email);
	}
	
	// read users from a file
	private void readExistingUsersFromFile() {
		try(FileInputStream fis = new FileInputStream(USER_FILE);
				Scanner scan = new Scanner(fis))	{  
			while(scan.hasNextLine()) {
				String line = scan.nextLine();
				//System.out.println(line);
				User u = parseUser(line);
				users.put(u.getEmail(), u);
			}

		} catch (FileNotFoundException e) {
			System.err.println("File not found exception in readExistingUsersFromFile");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("IOException in readExistingUsersFromFile");
			e.printStackTrace();
		}

	}
	
	/**
	 * helper method for reading users from a file
	 */
	private User parseUser(String line) {
		// need to know the file format to turn the line into a user
		User u = null;

		// email / name / password (security question / answer)
		Scanner parser = new Scanner(line);
		// in our data, tokens are separated by "/"
		parser.useDelimiter(",");
		// 3 tokens OR 5 tokens
		String status = parser.next();
		String email = parser.next();
		String name = parser.next();
		double money = parser.nextDouble();
		String password = parser.next();
		if(status.equals("Premium")) { 
			u = new PremUser(password, email, name, money);
		}
		else { // 3 tokens
			u = new FreeUser(password, email, name, money);
		}
		
		return u;
	}
	
	// write a single user to the file and not all of them
	public void writeSingleToFile(User u) {
		try ( FileOutputStream fos = new FileOutputStream("src/ExistingUsers", true);
				PrintWriter out = new PrintWriter(fos)){ // closes automatically
			out.println(u.getFileString()); 
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e1) {
			e1.printStackTrace();
		}

	}
	
	// write all users to file
	public void writeAllDataToFile() {
							// by adding true after the address, we are able to append at the end of file
		try ( FileOutputStream fos = new FileOutputStream("src/userInfo", true);
				PrintWriter out = new PrintWriter(fos)){ // closes automatically

			for(String email: users.keySet()) {
				User u = users.get(email);
				out.println(u.getFileString()); 
			}

		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}


	}

	// print out all users
	private void printUserMap() {
		String map = "All users (email --> user)\n";
		// go through the map, getting each entry
		for(String email: users.keySet()) {
			// given the key, get the value (user object)
			User u = users.get(email);
			map += "\t" + email + "-->" + u +"\n";
		}
		System.out.println(map);

	}
	
	// logging in for the user, create new user if not existent
	public User loggingIn() {
		//System.out.println("Logging in----------");
		String e = InputHelper.readString(sc, "What is your email?");
		String p = InputHelper.readString(sc, "Enter your password");
		
		User u = findUser(e);
		if(u == null) {
			System.out.println("User does not exist. ");
			boolean create = InputHelper.readYesNoBoolean(sc, "Would you like to create the user?");
			if(create) {
				u = createNewUser(p, e);
			}
		}else {
			while(!p.equals(u.getPassword())) {
				System.out.println("Wrong password!");
				p = InputHelper.readString(sc, "Please re-enter your password: ");
			}
			System.out.println("Log in completed! Welcome "+u.getName()+"!");
		}
		return u;
	}
	
	
	
	
	
	public void printMainMenu() {
		System.out.println("Main menu:");
		System.out.println("1) Log in");
		System.out.println("2) View parties hosted");
		System.out.println("3) View parties joined");
		System.out.println("4) Host a new party");
		System.out.println("5) Upgrade status");
		System.out.println("6) Store money");
		System.out.println("7) View balance");
		System.out.println("8) Log out");
		System.out.println("9) Quit");
	}
	
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		
		UserLogin system = new UserLogin();
		system.printUserMap();
		
		//system.createNewUser();
		
//		User u = system.findUser("varshi@usc.edu");
//		if(u == null) {
//			System.out.println("User does not exist. ");
//			boolean create = InputHelper.readYesNoBoolean(sc, "Would you like to create the user?");
//			if(create) {
//				system.createNewUser("varshi@usc.edu");
//			}
//		}
		
		system.printMainMenu();
		int menuC = InputHelper.readIntBetween(sc, "> ", 1, 9);
		if(menuC == 1) {
			system.loggingIn();
		}
		
		system.printUserMap();
		// write all data to a file
		system.writeAllDataToFile();
	}


}
