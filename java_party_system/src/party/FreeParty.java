package party;

/**
 * abstract class of free parties
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */
import java.util.*;
import user.*;

public abstract class FreeParty extends Party{
	int maxAttend;	// maximum attendees
	
	// constructors!!!
	public FreeParty(int m, String t, User h, ArrayList<User> ua) {
		super(t, h, ua);
		maxAttend = m;
	}
	
	public FreeParty(int m, String t, User h) {
		super(t, h);
		maxAttend = m;
	}
	
	public FreeParty(int m, String t) {
		super(t);
		maxAttend = m;
	}
	
	public FreeParty(String t) {
		super(t);
		maxAttend = 100;
	}
	
	// GETTER and SETTER
	public int getMax() {
		return maxAttend;
	}
	
	private void setMax(int m) {
		this.maxAttend = m;
	}

}
