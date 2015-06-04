/* 
 * This program is for verification of birthday paradox.
 * Experiment is performed on <people> number of people for <tries> number of times
 * Number of tries =1000. This value is hardcoded.
 * Output shows number of people , their mathematical result (using probability theory) and experimental result.
 * The mathematical and experimental results are close enough.... This verifies birthday paradox is valid.
 * 
 * */


import java.util.Random;


public class VerifyBirthdayParadox {

	// experimental result
	private static float expt_value(int people, int tries) {
		// TODO Auto-generated method stub
		int success=0;
		for(int j=0;j<tries;j++){
			success+=expt(people);
		}
		
		return (float)success/(float)tries;
	}

	private static int expt(int people) {
		// TODO Auto-generated method stub
		int[] ppl=new int[366];
		Random rand=new Random();
		int birthday;
		for (int j = 0; j < people; j++) {
			birthday=rand.nextInt(366);
			if (ppl[birthday] == 1)
				return 1;
			else {
				ppl[birthday] = 1;
			}
		}
		return 0;
	}

	//result using probrblity theory
	private static double probablity(int people) {
		// TODO Auto-generated method stub
		double num=365,num2=1;
		for(int j=1;j<=people;j++){
			num2=num2*(num-j+1);
		}
		double result= (num2/ Math.pow(365, people));
		if(Double.isNaN(result))
			return 1.0;
		
		return 1-result;
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		int tries=1000;
		System.out.format("%8s%18s%22s\n","people","probablity","experimental value");
		for(int people=1;people<366;people++){
			System.out.format("%8d%18f%22f\n", people,(float)probablity(people),expt_value(people, tries));
		}
	}


}
