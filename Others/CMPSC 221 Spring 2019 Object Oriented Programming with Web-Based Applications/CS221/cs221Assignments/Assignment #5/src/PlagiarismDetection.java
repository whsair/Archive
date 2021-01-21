

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeSet;

/**
 *
 * @author wangh_qx6
 */
public class PlagiarismDetection {

// Fig. 16.17: WordTypeCount.java
// Program counts the number of occurrences of each word in a String.
   public static void main(String[] args) throws FileNotFoundException {
      // create HashMap to store String keys and Integer values
      Map<String, Integer> map1 = new HashMap<>(); //create a hashmap for the first file
      Map<String, Integer> map2 = new HashMap<>(); //create a hashmap for the second file

      createMap(map1, args[0]); // create map1 based 1.text
      createMap(map2, args[1]); // create map2 based 2.text
      displayMap(map1, map2); // display map content and theirs Jaccard Index
      double jaccardIndex = compareMaps(map1, map2); //use compareMaps get the jaccardIndex

      //display the jaccard index and basic on the index in order to determine whether is it a plagiarism case
      System.out.printf("It is %s a plagiarism case. Similarity score = %s < threshold score %s\n", (jaccardIndex < Double.parseDouble(args[2]) ? "not" : ""), jaccardIndex, Double.parseDouble(args[2]));
   }

   // create map from user input
   private static void createMap(Map<String, Integer> map, String path) throws FileNotFoundException {

      // read record from file
      try (Scanner input = new Scanner(Paths.get("src\\" + path).toAbsolutePath())) {

         //designed a regular expression to deal with all kinds of punctuations in document
         input.useDelimiter("\\s+|(\\.|,|-|;|:|!|\\?|\"|\\[|\\]|\\(|\\)|'$|\\r|\\n)*(\\s)*(\\.|,|-|;|:|!|\\?|\"|\\[|\\]|\\(|\\)|'$|\\r|\\n)+(\\s)*");

         while (input.hasNext()) { // while there is more to read
            
            //get each tokens from scanner
            String word = input.next().toLowerCase(); // get lowercase word
            if (!word.isEmpty()) { // reject the breakspace and only add word in the map;
               // if the map contains the word
               if (map.containsKey(word)) { // is word in map?
                  int count = map.get(word); // get current count
                  map.put(word, count + 1); // increment count   
               } else {
                  map.put(word, 1); // add new word with a count of 1 to map
               }
            }

         }
      } catch (IOException | NoSuchElementException
              | IllegalStateException e) {
      }

   }

   //define a method called compareMaps, which takes as input two maps and return their Jaccard index (a double value). 
   private static double compareMaps(Map<String, Integer> map1, Map<String, Integer> map2) {
      Set<String> keys1 = map1.keySet(); // get map1's keys
      Set<String> keys2 = map2.keySet(); // get map2's keys
      //get map1 and map2's intersection keys  
      Set<String> intersection = new TreeSet<>(keys1); //create a hashset in order to get rid of the duplicate element in map1
      intersection.retainAll(keys2);   //merge the two set and also get rid of the duplicate element in map2

      // calculate the jaccard Index 
      return (double) (intersection.size()) / (map1.size() + map2.size() - intersection.size());

   }

   // display map content
   private static void displayMap(Map<String, Integer> map1, Map<String, Integer> map2) {
      Set<String> keys1 = map1.keySet(); // get map1's keys
      Set<String> keys2 = map2.keySet(); // get map2's keys
      //get map1 and map2's intersection keys  
      Set<String> intersection = new TreeSet<>(keys1); //create a TreeSet in order to get rid of the duplicate element in map1
      intersection.retainAll(keys2);   //merge the two set and also get rid of the duplicate element in map2

      int numberOfUniqueWordsInMAP1 = map1.size(); //Calculate the number of unique key in map1
      int numberOfUniqueWordsInMAP2 = map2.size(); //Calculate teh number of unique key in map2

      //display each number of unique key in maps and maps and also shows the number of common key in both set
      System.out.println("Number of unique words in document 1: " + numberOfUniqueWordsInMAP1);
      System.out.println("Number of unique words in document 2: " + numberOfUniqueWordsInMAP2);
      System.out.println("They have " + intersection.size() + " words in common");
      System.out.println("The common words and their counts in document 1 and document 2, respectively.");
      System.out.printf("%nWord\t\t  Count1\t\tCount2\n"); //print the tile

      // generate output for each key in map
      intersection.stream().filter((key) -> (!key.isEmpty())).forEachOrdered((key) -> {
         System.out.printf("%-13s%10s%20s%n", key, map1.get(key), map2.get(key));
      });   //print out each count corresponding to each interected key

   }
}
