package user;

/**
 * Prem User, so that they can host
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */

import java.util.*;
import party.*;
import system.InputHelper;

public class PremUser extends User{
	ArrayList<Party> hostHist = new ArrayList<Party>();	// list containing all the parties hosted
	
	// different constructors
	public PremUser(String p, String e, String n, double m, ArrayList<Party> jh, ArrayList<Party> hh) {
		super(p, e, n, m, jh);
		this.hostHist = hh;
	}
	
	public PremUser(String p, String e, String n, double m, ArrayList<Party> jh) {
		this(p,e, n, m, jh, new ArrayList<Party>());
	}
	
	public PremUser(String p, String e, String n, double m) {
		this(p,e, n, m, new ArrayList<Party>());
	}
	
	public PremUser(String p, String e, String n) {
		this(p, e, n, 0.0);
	}
	
	public PremUser(String p, String e) {
		this(p, e, "ANONYMOUS");
	}
	
	// GETTER
	public ArrayList<Party> getHosted(){
		return hostHist;
	}
	
	// for description
	public String toString() {
		return "Premium user: Name: "+this.getName()+"; Money: "+ this.getMoney() +"; Email: "+this.getEmail();
	}
	
	public void hostParty(Party e) {
		hostHist.add(e);
	}
	
	// for them to cancel membership and thus return a free user
	public FreeUser cancelMembership() {
		FreeUser f = new FreeUser(this.getPassword(),this.getEmail(), this.getName(), this.getMoney()+30.0, this.getJoin());
		return f;
	}
	
	// print out all the hosted parties (only available for premium user)
	public void printHostedParties() {
		System.out.println(this.getName()+"'s joined-party history:");
		for(Party p : hostHist) {
			System.out.println(p);
		}
	}
	
	// host tea party
	public Party hostTeaParty(Scanner sc, User u) {
		System.out.println("Welcome, it is time to host a tea party!");
		String title = InputHelper.readString(sc, "Think of a title for the party:");
		int m = InputHelper.readPositiveInt(sc, "At most, how many people can come?");
		TeaParty t = new TeaParty(m, title, u);
		t.chooseTea(sc);
		t.prepareBalloons(sc);
		
		System.out.println("\nYou are hosting "+t.getTitle()+", with a maximum of "+t.getMax()+" people coming and balloons that say: "+t.balloonToString()+".");
		System.out.println("Tea you ordered:");
		t.printTeaOrdered();
		
		this.hostParty(t);
		
		return t;
	}
	
	// host wine party
	public Party hostWineParty(Scanner sc, User u) {
		System.out.println("Welcome, it is time to host a wine tasting party!");
		String title = InputHelper.readString(sc, "Think of a title for the party:");
		double f = InputHelper.readPosDouble(sc, "How much should people pay if they want to come?");
		WineTasting w = new WineTasting(f, title, u);
		w.chooseWine(sc);
		
		System.out.println("\nYou are hosting "+w.getTitle()+", with a fee of $"+ w.getFee()+".");
		System.out.println("Wine you ordered:");
		w.printWineOrdered();
		
		this.hostParty(w);
		
		return w;
	}
	
	// host beer party
	public Party hostBeerParty(Scanner sc, User u) {
		System.out.println("Welcome, it is time to host a beer party!");
		String title = InputHelper.readString(sc, "Think of a title for the party:");
		int m = InputHelper.readPositiveInt(sc, "At most, how many people can come?");
		BeerParty t = new BeerParty(m, title, u);
		t.chooseBeer(sc);
		
		System.out.println("\nYou are hosting "+t.getTitle()+", with a maximum of "+t.getMax()+" people coming.");
		System.out.println("Beer you ordered:");
		t.printBeerOrdered();
		
		this.hostParty(t);
		
		return t;
	}
}
