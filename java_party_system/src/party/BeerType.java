package party;

/**
 * enum for different beer type
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */
public enum BeerType {
	// all the types of beer available 
	American,
	British,
	Chinese,
	Korean;
	
	/**
	 * print out all the beer types for the host to choose from
	 */
	public static void printBeer() {
		System.out.println("Here are the available beer options for the party: ");
		int counter = 1;
		for(BeerType t: BeerType.values()) {
			System.out.println(counter+") "+t);
			counter ++;
		}
	}
}
