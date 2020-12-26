package party;

/**
 * wine tasting, a paid party
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.*;

import system.InputHelper;
import user.*;

public class WineTasting extends PaidParty{
	private HashMap<String, Double> wineOrdered;					// wine ordered
	private ArrayList<String> allWine = new ArrayList<String>();	// to record all wine String from wineInfo
	private ArrayList<Double> allPrices = new ArrayList<Double>();	// all prices from wineInfo
	
	/**
	 * how the party is to be experienced
	 */
	public void experienceParty(Scanner sc, User u) {
		System.out.println("\nWelcome to "+this.getHost().getName()+"'s Wine Tasting: "+this.getTitle());
		System.out.println("\tYou noticed the wine they provide: ");
		for(String wine: wineOrdered.keySet()) {
			System.out.println("\t\t"+wine+", $"+wineOrdered.get(wine));
		}
		
		boolean purchase = InputHelper.readYesNoBoolean(sc, "Do you want to purchase any wine provided here?");
		if(purchase) {
			this.sellWine(sc, u);
		}
		
		System.out.println("\n\tYou had a good time.");
	}
	
	/**
	 * for file description
	 */
	public String getFullDescription() {
		String a = "WineTasting,"+this.getTitle()+","+this.getHost()+","+wineOrdered+","+userAttended;
//		for(String w: wineOrdered.keySet()) {
//			a += w+"/" +wineOrdered.get(w)+"//";
//		}
//		for (User u:userAttended) {
//			a += u.getJoin();
//		}
		return a;
	}
	
	// consturctors
	public WineTasting(double f, String t, User h, ArrayList<User> ua) {
		super(f, t, h, ua);
		wineOrdered = new HashMap<String, Double>();
	}
	
	public WineTasting(double f, String t, User h) {
		this(f, t, h, new ArrayList<User>());
	}
	
	public WineTasting(double f, String t) {
		this(f, t, new FreeUser());
	}
	
	public WineTasting(String t) {
		this(100.0, t);
	}
	
	// print out the ordered wine
	public HashMap<Integer, String> printWineOrdered() {
		HashMap<Integer, String> c = new HashMap<Integer, String>();
		
		int i = 1;
		for (String wineName : wineOrdered.keySet()) {
			System.out.println(i + ") "+ wineName+", $"+wineOrdered.get(wineName));
			c.put(i, wineName);
			i++;
		}
		
		return c;
	}
	
	// read wine name and price from file
	public void populateWine() {
		String fileName = "src/wineInfo";
		try(FileInputStream fis = new FileInputStream(fileName);
				Scanner sc = new Scanner(fis))	{  
			while(sc.hasNextLine()) {
				String line = sc.nextLine();
				Scanner parser = new Scanner(line);
				parser.useDelimiter(",");
				allWine.add(parser.next());
				allPrices.add(parser.nextDouble());
			}

		} catch (FileNotFoundException e) {
			System.err.println("File not found exception in readExistingUsersFromFile");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("IOException in readExistingUsersFromFile");
			e.printStackTrace();
		}
	}
	
	
	// read wine name and prices from a file and append them to hash map
	// make sure this is what the user want
	public void chooseWine(Scanner sc) {
		this.populateWine();
		
		boolean cont = true;
		
		do {
			// print out all the wine available from collection
			System.out.println("\nHere are all the wine you can choose from for your party: ");
			int counter = 1;
			for(String w: allWine) {
				System.out.println(counter + ") " + w);
				counter ++;
			}
			
			int wine = InputHelper.readIntBetween(sc, "> ", 1, 6);
			// add if it is not contained already
			if(wineOrdered.putIfAbsent(allWine.get(wine-1),allPrices.get(wine-1))!=null){
				System.out.println("You have selected this before.");
			}
			
			if (wineOrdered.size() == 6) {
				System.out.println("Friendly reminder, you have now chosen all wine types!\n");
			}
			
			// let user be able to edit their tea menu until finalized
			System.out.println("\nTell us your next move: \n1) Add other wine type\n2) Remove wine type\n3) Quit, this is my finalized wine menu");
			int move = InputHelper.readIntBetween(sc, "> ", 1, 3);
			
			// if quit, update loop's condition; otherwise, help with removing the tea chosen; if they want to add more, 
			if (move == 3) {
//				System.out.println("\nHere is the wine provieded at the party:");
//				printWineOrdered();
				cont = false;
			}else if (move == 2) {
				System.out.println("\nHere are the wine provided at your party?");
				HashMap<Integer, String> wineList = printWineOrdered();
				int remove = InputHelper.readIntBetween(sc, "> ", 1, wineOrdered.size());
				
				wineOrdered.remove(wineList.get(remove),wineOrdered.get(wineList.get(remove)));
				
				cont = !InputHelper.readYesNoBoolean(sc, "Would you like to quit customizing your wine menu now?");
				if(!cont && wineOrdered.size() == 0) {
					cont = true;
					System.out.println("You need to order at least one wine for your party!");
				}
			}
						
		}while(cont);
		
	}
	
	// sell wine to guests
	public void sellWine(Scanner sc, User u) {
		boolean cont = true;
		do {
			System.out.println("\nParty's Wine List:");
			HashMap<Integer, String> wineList = printWineOrdered();
			int buy = InputHelper.readIntBetween(sc, "Which wine option would you like to purchase?", 1, wineOrdered.size());
			int num = InputHelper.readPositiveInt(sc, "How many bottles would you like to purchase (0 to infinite)?");
			double total = num* wineOrdered.get(wineList.get(buy));
			System.out.println("You have purchased "+num+" bottles of "+wineList.get(buy)+" at a total of $"+total);
			u.spendMoney(total);
			
			
			// let user be able to edit their tea menu until finalized
			System.out.println("\nTell us your next move: \n1) Buy another\n2) Quit, leave party and will buy no more wine");
			int move = InputHelper.readIntBetween(sc, "> ", 1, 2);
			// if quit, update loop's condition; otherwise, help with removing the tea chosen; if they want to add more, 
			if (move == 2) {
				System.out.println("\nThank you for coming to the party <3");
				cont = false;
			}
		}while(cont);
		
	}
	
	
//	public static void main(String[] args) {
//		WineTasting w = new WineTasting("Joyous!");
//		Scanner sc = new Scanner(System.in);
//		FreeUser u = new FreeUser("e", "kkk", 5000.0);
//		w.chooseWine(sc);
//		w.sellWine(sc, u);
//	}
	
}
