package party;

/**
 * tea party, concrete class of Free Party
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */
import java.util.*;

import system.InputHelper;
import user.*;

public class TeaParty extends FreeParty implements NonAlcoholic{
	ArrayList<TeaType> teaOrdered;	// what has been ordered
	String balloon = null;	// what says on baloon
	
	// how it is experienced
	public void experienceParty(Scanner sc, User u) {
		System.out.println("\nWelcome to "+this.getHost().getName()+"'s Tea Party: "+this.getTitle());
		System.out.println("\tYou see balloons that say "+ this.balloonToString() +" on it.");
		System.out.println("\tYou noticed the tea they provide:");
		for(TeaType t: teaOrdered) {
			System.out.println("\t\t"+t);
		}
		System.out.println("\n\tYou had a good time.");
	}
	
	/**
	 * when not customized, balloon says nothing
	 * @return
	 */
	public String balloonToString() {
		if (balloon.equals("") || balloon == null) {
			return "nothing";
		}else {
			return balloon;
		}
	}
	
	/**
	 * for file description
	 */
	public String getFullDescription() {
		String a = "TeaParty,"+this.getTitle()+","+this.getHost()+",";
		a+= teaOrdered+","+userAttended+","+this.getMax()+","+this.balloonToString();
//		for(TeaType t:teaOrdered) {
//			a += "\n\t\t"+t;
//		}
//		
//		for (User u:userAttended) {
//			a += "\n\t\t"+u.getName()+", "+u.getEmail();
//		}
		return a;
	}
	
	// constructors
	public TeaParty(int m, String t, User h, ArrayList<User> ua) {
		super(m, t, h, ua);
		teaOrdered = new ArrayList<TeaType>();
	}
	
	public TeaParty(int m, String t, User h) {
		super(m, t, h);
		teaOrdered = new ArrayList<TeaType>();
	}
	
	public TeaParty(int m, String t) {
		super(m, t);
		teaOrdered = new ArrayList<TeaType>();
	}
	
	public TeaParty(String t) {
		super(t);
		teaOrdered = new ArrayList<TeaType>();
	}
	
	public void printTeaOrdered() {
//		System.out.println("Here is the tea provided at the party:");
		int counter = 1;
		for (TeaType t: teaOrdered) {
			System.out.println(counter + ") "+ t);
			counter ++;
		}
	}
	
	// company encourages non alcoholic parties, therefore we will send balloon free of charge
	// if host wants balloon sent, they can customize something on the balloon
	public void prepareBalloons(Scanner sc) {
		System.out.println("For hosing non-alcoholic parties, you may enjoy our free balloons!");
		balloon = InputHelper.readString(sc, "Please customize your balloon with a brief sentence (or press ENTER to not customize)") ;
	}
	
	// make user choose from all four tea type enum and append them to the class's tea list
	public void chooseTea(Scanner sc) {
		boolean cont = true;
		
		do {
			// print out all the enums
			TeaType.printTea();
			// ask user to choose from enums
			int tea = InputHelper.readIntBetween(sc, "> ", 1, 4);
			// add if it is not contained already
			if(!teaOrdered.contains(TeaType.values()[tea-1])) {
				teaOrdered.add(TeaType.values()[tea-1]);
			}else {
				System.out.println("You have selected this before.");
			}
			
			if (teaOrdered.size() == 4) {
				System.out.println("Friendly reminder, you have now chosen all tea types!");
			}
			
			// let user be able to edit their tea menu until finalized
			System.out.println("Tell us your next move: \n1) Add more tea type\n2) Remove tea type\n3) Quit, this is my finalized tea menu");
			int move = InputHelper.readIntBetween(sc, "> ", 1, 3);
			
			// if quit, update loop's condition; otherwise, help with removing the tea chosen; if they want to add more, 
			if (move == 3) {
				cont = false;
			}else if (move == 2) {
				System.out.println("Here is the tea provided at the party:");
				this.printTeaOrdered();
				System.out.println("Which tea do you want to remove?");
				int remove = InputHelper.readIntBetween(sc, "> ", 1, teaOrdered.size());
				teaOrdered.remove(remove-1);
				
				cont = !InputHelper.readYesNoBoolean(sc, "Would you like to quit customizing your tea menu now?");
				if(!cont && teaOrdered.size() == 0) {
					cont = true;
					System.out.println("You need to order at least one tea for your party!");
				}
			}
						
		}while(cont);
		
	}



}
