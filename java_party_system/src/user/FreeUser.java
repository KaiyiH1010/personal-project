package user;


/**
 * free user, who cannot host
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */

import java.util.*;
import party.*;

public class FreeUser extends User{
	// the different constructors
	public FreeUser(String p, String e, String n, double m, ArrayList<Party> j) {
		super(p,e, n, m, j);
	}
	
	public FreeUser(String p, String e, String n, double m) {
		super(p, e, n, m);
	}
	
	public FreeUser(String p, String e, String n) {
		super(p, e, n);
	}
	
	public FreeUser(String p, String e) {
		super(p, e);
	}
	
	public FreeUser() {
		super("password?","UNAVAILABLE", "ANONYMOUS", -1.0, null);
	}
	
	// for description use
	public String toString() {
		return "Free user:    Name: "+this.getName()+"; Money: "+ this.getMoney()+"; Email: " + this.getEmail();
	}
	
	// when they become a member, return a premium user if they have enough money to complete the upgrade
	public PremUser becomeMember() {
		if (this.spendMoney(50.0)) {
			PremUser p = new PremUser(this.getPassword(),this.getEmail(), this.getName(), this.getMoney(), this.getJoin(), new ArrayList<Party>());
			return p;
		}else {
			return null;
		}
	}
}
