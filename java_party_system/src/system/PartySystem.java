package system;

/**
 * ParySystem. basically the main class
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.*;
import user.*;
import party.*;

public class PartySystem {
	private User u;						// the current user
	private ArrayList<Party> parties;	// 
	private ArrayList<User> allUser; //replace this with UserLogin's user HashMap <String email, User u>
	private UserLogin ul;
	
	// constructor
	public PartySystem() {
		parties = new ArrayList<Party>();
		allUser = new ArrayList<User>();
		ul = new UserLogin();
		
		// get an initial user, and then logged = true
	}

	// save all the parties into PartyInfo
	private void saveParties() {
		try ( FileOutputStream fos = new FileOutputStream("src/PartyInfo", true);
				PrintWriter out = new PrintWriter(fos)){ // closes automatically
			for(Party p: parties) {
				out.println(p.getFullDescription()); 
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e1) {
			e1.printStackTrace();
		}
	}
	
	// save all the users into the file
	private void saveUsers() {
		ul.writeAllDataToFile();
	}
	
	// print out all main menu
	public void printMainMenu() {
		System.out.println("\nMain menu for "+u.getStatus()+" user "+u.getName()+":");
		System.out.println("1) Log in");
		System.out.println("2) View parties joined");
		System.out.println("3) View parties hosted");
		System.out.println("4) Host a new party");
		System.out.println("5) Update status");	
		System.out.println("6) Store money");
		System.out.println("7) View balance");
		System.out.println("8) Attend available parties");	//NEED WORK
		System.out.println("9) Log out");
		System.out.println("10) Quit\n");
	}
	
	// upgrade the user's status
	public User upgradeStatus(Scanner sc, User u) {
		if(u instanceof PremUser) {
			System.out.println("You are currently a premium user, would you like to cancel your membership?");
			System.out.println("You will only get 30 (out of 50) dollar of refund back into your account, but you will also lose all your perks.\nDo you truly wish to cancel?");
			boolean response = InputHelper.readYesNoBoolean(sc, "> ");
			if(response) {
				u = ((PremUser) u).cancelMembership();
			}
		}else {
			System.out.println("You are currently a free user, would you like to subscribe to membership?");
			System.out.println("It'll cost 50 dollars but you will be able to host parties and enjoy a 20% discount of attendance fee for paid parties.\nDo you want to become a member?");
			boolean response = InputHelper.readYesNoBoolean(sc, "> ");
			if(response) {
				u = ((FreeUser)u).becomeMember();
			}
		}
		
		return u;
	}
	
	// help host the party depending on request
	public void hostingParty(Scanner sc, PremUser u) {
		int par = InputHelper.readIntBetween(sc, "Would you like to host a 1) tea party or a 2) wine tasting or a 3) beer party?", 1, 3);
		if(par==1) {
			parties.add(u.hostTeaParty(sc, u));
		}else if (par == 2){
			parties.add(u.hostWineParty(sc, u));
		}else {
			parties.add(u.hostBeerParty(sc, u));
		}
		
		System.out.println("\nYou have successfully hosted a party. Other users may join it by using main menu's option 8.");
	}
	
	// help user join the right party
	public void joiningParty(Scanner sc, User u) {
		// list out all the parties held
		int counter = 1;
		for (Party p :parties) {
			System.out.println(counter +") "+p.getPartyType()+": "+p.getTitle()+", hosted by "+p.getHost().getName() 
					+" and currently has "+p.getAttended().size()+" guests attending.");
			if ( p instanceof PaidParty) {
				System.out.println("\tFee: $"+ ((PaidParty)p).getFee()+" for Free User and $"+ ((PaidParty)p).getFee()*0.8 +" for Premium User");
			}
			counter ++;
		}
		// understand what user want
		int par = InputHelper.readIntBetween(sc, "Which party do you want to attend? Type 0 to go back to main menu", 0, parties.size());
		
		if(par !=0 ) {
			// the party user want to attend to is parties.get(par-1)
			
			if (parties.get(par-1) instanceof TeaParty) {
				if(u.attendParty(parties.get(par-1))) {
					parties.get(par-1).experienceParty(sc, u);
				}
			}else {
				// when it is wine tasting and it is a paid event
				if(u.attendParty(parties.get(par-1))) {
					parties.get(par-1).experienceParty(sc, u);
				}
			}
			
		}
		
		
	}
	
	// run the program
	public void run() {
		// make sure someone logs in from the very beginning
		System.out.println("Welcome stranger, please log in to begin...");
		u = ul.loggingIn();
		User anonymous = new FreeUser();
		
		Scanner sc = new Scanner(System.in);
		
		boolean cont = true;
		while(cont) {
			printMainMenu();
			int main = InputHelper.readIntBetween(sc, "> ", 1, 10);
			switch(main) {
				case 1:
					if (u != anonymous) {
						System.out.println("You are already logged in.");
					}else {
						u = ul.loggingIn();
					}
					break;
				case 2:
					if (u != anonymous) {
						u.printJoinedParty();
					}else {
						System.out.println("Please log into an account.");
					}
					break;
				case 3:
					if (u != anonymous) {
						if(u instanceof PremUser) {
							((PremUser) u).printHostedParties();
						}else {
							System.out.println("Function only available for premium users. You are currently a free user.");
						}
					}else {
						System.out.println("Please log into an account.");
					}
					break;
				case 4:
					if (u != anonymous) {
						if(u instanceof PremUser) {
							hostingParty(sc, (PremUser) u);
						}else {
							System.out.println("Function only available for premium users. You are currently a free user.");
						}
					}else {
						System.out.println("Please log into an account.");
					}
					break;
				case 5:
					if (u != anonymous) {
						User newU = this.upgradeStatus(sc, u);
						ul.getUsers().replace(u.getEmail(), newU);
						u = newU;
					}else {
						System.out.println("Please log into an account.");
					}
					break;
				case 6:
					if (u != anonymous) {
						u.storeMoney(sc);
					}else {
						System.out.println("Please log into an account.");
					}
					break;
				case 7:
					if (u != anonymous) {
						System.out.println(u.getName()+", you currently have $"+u.getMoney()+" in your account.");
					}else {
						System.out.println("Please log into an account.");
					}
					break;
				case 8:
					if (u != anonymous) {
						if(parties.size() == 0) {
							System.out.println("Oops! We currently have no parties hosted!");
						}else {
							this.joiningParty(sc, u);	
						}
					}else {
						System.out.println("Please log into an account.");
					}
					break;
				case 9:
					if(u!=anonymous) {
						System.out.println(u.getName()+", you are now logged off.");
						u = anonymous;
					}else {
						System.out.println("You are already logged off.");
					}
					break;
				case 10:
					// save everything before ending the program
					this.saveParties();
					ul.writeAllDataToFile();
					//goodbye
					System.out.println("Thank you for partying with us! Hope to see you again soon.\n\tGoodbye");
					cont = false;
					break;
				default:
					break;
			}
		}
	}
	
	
	public static void main(String[] args) {
		PartySystem ps = new PartySystem();
		ps.run();
	}

}

