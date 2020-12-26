package party;

/**
 * tea enums
 *
 * @author Kaiyi Huang
 * ITP 265, Fall 2019, Class Section Tea
 * Final Project
 * Email: kaiyihua@usc.edu
 */
public enum TeaType {
	Oolong,
	Matcha,
	Earl,
	Cranberry;
	
	// print out all the tea
	public static void printTea() {
		System.out.println("Here are the available tea options for the party: ");
		int counter = 1;
		for(TeaType t: TeaType.values()) {
			System.out.println(counter+") "+t);
			counter ++;
		}
	}

}
