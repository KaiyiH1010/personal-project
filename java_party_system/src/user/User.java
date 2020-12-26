package user;

/**
 * User superclass
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */

import java.util.*;

import party.*;

public abstract class User{
	String fileString = "src/userInfo";
	String name;				// user name
	String email;				// email
	String password;			// password
	double money;				// money stored in the account
	ArrayList<Party> joinHist;	// history of the parties joined
	
	// the different constructors
	public User(String p, String e, String n, double m, ArrayList<Party> j) {
		this.password = p;
		this.email=e;
		this.name = n;
		this.money = m;
		this.joinHist = j;
	}
	
	public User(String p, String e, String n, double m) {
		this(p, e, n, m, new ArrayList<Party>());
	}
	
	public User(String p, String e, String n) {
		this(p, e, n, 0.0);
	}
	
	public User(String p, String e) {
		this(p, e, "ANONYMOUS");
	}
	
	// abstract method for subclasses to implement
	public abstract String toString();
	
	// attend party by adding party into user's party list and update party's user list
	public boolean attendParty(Party e) {
		if(this.equals(e.getHost())) {
			System.out.println("Host cannot attend this party as guests.");
			return false;
		}
		
		if(e.getAttended().contains(this)) {
			System.out.println("You already attended this party.");
			return false;
		}
		
		boolean canAttend = false;
		if (e instanceof PaidParty) {
			// need to check user's bank account
			if(this instanceof PremUser) {
				// they get a 20% off discount
				canAttend = this.spendMoney(((PaidParty) e).getFee()*0.8);
			}else {
				canAttend = this.spendMoney(((PaidParty) e).getFee());
			}
		}else if(e instanceof FreeParty){
			// if this is free party, check to see if we have reached maximum attendance yet
			if(((FreeParty) e).getMax()>e.getAttended().size()) {
				canAttend = true;
			}else {
				System.out.println("Sorry, this party is full! Better luck next time.");
			}
		}
		
		if(canAttend) {
			joinHist.add(e);
			e.userJoining(this);
		}
		return canAttend;
	}
	
	// check if there is enough money to spend, if yes then spend the money, if not return false
	public boolean spendMoney(double m) {
		if (money < m) {
			System.out.println("No trasaction will occur due to insufficient fund.");
			return false;
		}else {
			money -= m;
			return true;
		}
	}
	
	/**
	 * to store money into the account
	 * @param m
	 */
	public void storeMoney(Scanner sc) {
		double m = InputHelper.readPosDouble(sc, "You currently have $"+this.getMoney()+ ".How much US dollars would you like to store into your account?");
		money += m;
	}
	
	/**
	 * return true or false on judging two users are the same or not based on their names
	 */
	public boolean equals(Object o) {
		if (o instanceof User && ((User) o).getEmail() == this.getEmail()) {
			return true;
		}else {
			return false;
		}
	}
	
	// ALL GETTER METHODS
	public String getEmail() {
		return email;
	}
	
	public String getName() {
		return name;
	}
	
	public double getMoney() {
		return money;
	}
	
	public ArrayList<Party> getJoin(){
		return joinHist;
	}
	
	public String getStatus() {
		if (this instanceof PremUser) {
			return "Premium";
		}else {
			return "Free";
		}
	}
	
	protected String getPassword() {
		return password;
	}
	
	// ALL SETTER METHODS THAT MAKES SENSE (email shouldn't be changed, we have spend/store money, we have attend party)
	public void setName(String n) {
		name = n;
	}
	
	// for description
	public String getFileString() {
		String status = "";
		if (this instanceof PremUser) {
			status = "Premium";
		}else {
			status = "Free";
		}
		return status+","+this.email+","+this.getName()+","+this.getMoney()+","+this.getPassword();
	}
	
	
	public void printJoinedParty() {
		System.out.println(this.getName()+"'s joined-party history:");
		for(Party p : joinHist) {
			System.out.println(p);
		}
	}
}
