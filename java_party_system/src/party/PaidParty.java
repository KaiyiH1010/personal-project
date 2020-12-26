package party;

/**
 * parties that need to be paid to get in
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */
import java.util.*;
import user.*;

public abstract class PaidParty extends Party{
	double fee;	// it is a paid party
	
	// constructors
	public PaidParty(double f, String t, User h, ArrayList<User> ua) {
		super(t, h, ua);
		fee = f;
	}
	
	public PaidParty(double f, String t, User h) {
		super(t, h);
		fee = f;
	}
	
	public PaidParty(double f, String t) {
		super(t);
		fee = f;
	}
	
	public PaidParty(String t) {
		super(t);
		fee = 100.0;
	}
	
	
	// GETTER AND SETTERS
	public double getFee() {
		return fee;
	}
	
	private void setFee(double f) {
		fee = f;
	}
	
	
}
