package party;

/**
 * abstract party, superclass for all parties
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */
import java.util.*;
import user.*;

public abstract class Party {
	String title;	// the title
	User host;	// who host this	
	ArrayList<User> userAttended;	// who are the guests
	
	// abstract method because every party type have different characteristics
	public  abstract String getFullDescription();
	
	// every party type is experienced differently
	public abstract void experienceParty(Scanner sc, User u);

	// constructors
	public Party(String t, User h, ArrayList<User> ua) {
		title = t;
		host = h;
		this.userAttended = ua;
	}
	
	public Party(String t, User h) {
		this(t, h, new ArrayList<User>());
	}
	
	public Party(String t) {
		this(t, new FreeUser());
	}
	
	// how to print out the party
	public String toString() {
		return this.getPartyType()+": "+title +" hosted by "+ host.getName();
	}
	
	//GETTER AND SETTERS
	public void setTitle(String n) {
		title = n;
	}
	
	public String getTitle() {
		return title;
	}
	
	public User getHost() {
		return host;
	}
	
	public ArrayList<User> getAttended(){
		return this.userAttended;
	}
	
	// test whether this is the same party or not
	public boolean equals(Object o) {
		if (o instanceof Party 
				&& ((Party) o).getTitle() == this.title 
				&& ((Party) o).getHost() == this.host) {
			return true;
		}else {
			return false;
		}
	}
	
	// get a string describing what type of party it is 
	public String getPartyType() {
		if (this instanceof TeaParty) {
			return "Tea Party";
		}else if (this instanceof WineTasting) {
			return "Wine Tasting";
		}else if (this instanceof BeerParty) {
			return "Beer Party";
		}
		return "";
	}
	
	// what to do when a user joined
	public void userJoining(User u) {
		userAttended.add(u);
	}
}
