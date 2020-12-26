package system;

import java.util.Scanner;


/**
 * help with getting the desired input
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */

public class InputHelper {

	public static String readString(Scanner sc, String promptToPrint) {
		System.out.println(promptToPrint);
		// assume any string is okay (for now)
		return sc.nextLine();
	}

	/*Displays the prompt in the console, then waits for the user to enter an integer and returns 
	 * the integer. If a user enters an incorrect value such as letters or the alphabet, then a 
	 * message is printed saying why the value is invalid and the user is prompted to enter another 
	 * integer. This should continue until the user input is an integer, at which point the value is 
	 * returned.
	 * 
	 */
	public static int readInt(Scanner sc, String prompt) {
		System.out.println(prompt); // the question to ask the user
		int num = 0; // declare variable to hold user's input
		while (! sc.hasNextInt()) {  // sc.hasNextInt() == false
			// the user entered something that was NOT an int
			String garbage  = sc.nextLine(); // whatever the user hit (and the enter after what they hit)
			// should be completely skipped so we can eventually read an int
			System.out.println("Sorry, you entered " + garbage + " which is not an integer value.");
			System.out.println(prompt); // repeat what you want
		}

		num = sc.nextInt();

		sc.nextLine(); // move the scanner past the "enter/return" after the number

		return num;

	}
	
	public static double readDouble(Scanner sc, String prompt) {
		System.out.println(prompt); 
		double num = 0; 
		while (! sc.hasNextDouble()) {  
			String garbage  = sc.nextLine();
			System.out.println("Sorry, you entered " + garbage + " which is not a double value.");
			System.out.println(prompt); 
		}

		num = sc.nextDouble();
		sc.nextLine(); 
		return num;
	}
	
	
	public static int readIntBetween(Scanner sc, String prompt, int minInt, int maxInt) {
		int num = readInt(sc,prompt);
		while(num< minInt || num > maxInt) {
			System.out.println("Enter an integer between " + minInt + " and " + maxInt);
			num = readInt(sc, prompt);
		}

		return num; 


		/* Displays the prompt in the console, then waits for the user to enter an integer between 
		 * the int values of minInt and maxInt and returns the integer. If a user enters an incorrect 
		 * value such as letters or the alphabet or with a value below minInt or above maxInt, then a 
		 * message is printed saying why the value is invalid and the user is prompted to enter another integer. This should continue until the user input is within the appropriate range, at which point the value is returned.
		 */
	}
	public static int readPositiveInt(Scanner sc, String prompt) {
		int info = readInt(sc,prompt);
		while(info < 0) {
			System.out.println("Enter a positive integer");
			info = readInt(sc, prompt);
		}

		return info; 

	}
	
	public static double readPosDouble(Scanner sc, String prompt) {
		double info = readDouble(sc,prompt);
		while(info < 0) {
			System.out.println("Please nter a positive double");
			info = readDouble(sc, prompt);
		}
		return info; 
	}
	
	public static boolean readBoolean(Scanner sc, String question) { 
		System.out.println(question); // the question to ask the user
		boolean answer = false; // declare variable to hold user's input
		while (! sc.hasNextBoolean()) {  
			// the user entered something that was NOT an boolean
			String garbage  = sc.nextLine(); // whatever the user hit (and the enter after what they hit)
			// should be completely skipped so we can eventually read an int
			System.out.println("Sorry, you entered " + garbage + " which is not an integer value.");
			System.out.println(question); // repeat what you want
		}

		answer = sc.nextBoolean();

		sc.nextLine(); // move the scanner past the "enter/return" after the number

		return answer;
		
	
	}
	
	
	public static boolean readYesNoBoolean(Scanner sc, String question) {
		
		String answer = getYesOrNoString(sc, question);
		
		if(isYes(answer)) {
			return true;
		}
		else
			return false;
	
		/* Displays the question in the console, then waits for the user to enter yes or no (could be 
		 * Y/y/yes or N/n/no) as an answer. If a user enters a value other than Y/y/yes or N/n/no, 
		 * then a message is printed saying that the value wasn’t recognized as yes or no and the 
		 * user is prompted to enter another answer. This should continue until the user input matches yes or no, at which point the method should return a boolean (true for Y/y/yes 
		 * and false for N/n/no).
		 */
	}
	/**
	 * Returns true if the input matches "yes" or "y" (any capitalization) - or starts with Y
	 * @param input
	 * @return
	 */
	private static boolean isYes(String input) {
		if (input.toUpperCase().startsWith("Y")) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * Returns true if the input matches "no" or "n" (any capitalization) - or starts with N
	 * @param input
	 * @return
	 */
	private static boolean isNo(String input) {
		if (input.toUpperCase().startsWith("N")) {
			return true;
		}
		else {
			return false;
		}
	}


	/**
	 * @param sc
	 * @param question
	 * @return -- will always be a String of either "yes" or "no"
	 */
	public static String getYesOrNoString(Scanner sc, String question) {
		String answer = "";
		
		System.out.println(question);
		answer = sc.nextLine(); // get user input
		while(!isYes(answer) && !isNo(answer)) {
			System.out.println("didn't recognize " + answer + " as a yes or no");
			System.out.println(question);
			answer = sc.nextLine(); // get user input
		}
		
		return answer;
	}



	public static void main(String[] args) {
		/*This method is for testing purposes. Set up a Scanner object and then call each of the 
		 * methods, making sure they work as expected. 
		 * 
		 */
	}

	
	public static void printStars(int num) {
		for(int i = 0; i < num; i++) {
			System.out.print("*");
		}
		System.out.println();
		
	}

}