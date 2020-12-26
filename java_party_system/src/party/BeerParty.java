package party;

/**
 * beer party, free 
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */
import java.util.*;

import system.InputHelper;
import user.*;

public class BeerParty extends FreeParty{
	ArrayList<BeerType> beerOrdered;
	
	/**
	 * constructors!!!
	 * @param m
	 * @param t
	 * @param h
	 * @param ua
	 */
	public BeerParty(int m, String t, User h, ArrayList<User> ua) {
		super(m, t, h, ua);
		beerOrdered = new ArrayList<BeerType>();
	}
	
	public BeerParty(int m, String t, User h) {
		super(m, t, h);
		beerOrdered = new ArrayList<BeerType>();
	}
	
	public BeerParty(int m, String t) {
		super(m, t);
		beerOrdered = new ArrayList<BeerType>();
	}
	
	public BeerParty(String t) {
		super(t);
		beerOrdered = new ArrayList<BeerType>();
	}
	
	public void printBeerOrdered() {
		int counter = 1;
		for (BeerType t: beerOrdered) {
			System.out.println(counter + ") "+ t);
			counter ++;
		}
	}
	
	/**
	 * how a guest would experience the party
	 */
	public void experienceParty(Scanner sc, User u) {
		System.out.println("\nWelcome to "+this.getHost().getName()+"'s Beer Party: "+this.getTitle());
		System.out.println("\tYou noticed the beer they provide:");
		for(BeerType t: beerOrdered) {
			System.out.println("\t\t"+t);
		}
		System.out.println("\n\tYou had a good time.");
	}
	
	/**
	 * return a string for parties to be written in file
	 */
	public String getFullDescription() {
		String a = "BeerParty,"+this.getTitle()+","+this.getHost()+",";
		a+= beerOrdered+","+userAttended+","+this.getMax();
		return a;
	}
	
	/**
	 * to help host choose what beer they want to have for their party
	 * @param sc
	 */
	public void chooseBeer(Scanner sc) {
		boolean cont = true;
		
		do {
			BeerType.printBeer();
			int beer = InputHelper.readIntBetween(sc, "> ", 1, 4);
			// add if it is not contained already
			if(!beerOrdered.contains(BeerType.values()[beer-1])) {
				beerOrdered.add(BeerType.values()[beer-1]);
			}else {
				System.out.println("You have selected this before.");
			}
			
			if (beerOrdered.size() == 4) {
				System.out.println("Friendly reminder, you have now chosen all tea types!");
			}
			
			// let user be able to edit their beer menu until finalized
			System.out.println("Tell us your next move: \n1) Add more beer type\n2) Remove beer type\n3) Quit, this is my finalized beer menu");
			int move = InputHelper.readIntBetween(sc, "> ", 1, 3);
			
			// if quit, update loop's condition; otherwise, help with removing the tea chosen; if they want to add more
			if (move == 3) {
				cont = false;
			}else if (move == 2) {
				System.out.println("Here is the beer provided at the party:");
				this.printBeerOrdered();
				System.out.println("Which beer do you want to remove?");
				int remove = InputHelper.readIntBetween(sc, "> ", 1, beerOrdered.size());
				beerOrdered.remove(remove-1);
				
				cont = !InputHelper.readYesNoBoolean(sc, "Would you like to quit customizing your beer menu now?");
				if(!cont && beerOrdered.size() == 0) {
					cont = true;
					System.out.println("You need to order at least one beer for your party!");
				}
			}
						
		}while(cont);
		
	}

}
