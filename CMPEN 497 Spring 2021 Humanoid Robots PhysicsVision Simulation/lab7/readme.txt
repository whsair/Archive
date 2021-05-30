Name:Hongshuo Wang
Lab7 report
(0,0) locates at top left corner

Implementation details:

1. load the data into pandas dataframe

2. create six columns' varible each corresponding to

['segment name', 'proximal name', 'distal name', 'segment CoM x', 'segment CoM y', weight]

3. we can use provided prox_dist_pairs to get the index of proximal and distal in each segment
then using the equation given in the lab:

 CoM location is (1-%length)*location of proximal + %length*location of distal

loop through the whole prox_dist_pairs in order to get each segment's output information

4. The overall center of Mass = sum(each segment location * %weight)

###
Does the center of mass that you computed support that idea? Feel free to use additional information
from the image so justify your response. If your response is yes, explain why. If your response
is no, list some possible reasons for this disagreement.
###

Yes, my final output (overall center of Mass) is (115, 96). By looking back to the image we can see
this location is at the core of the person in the image which is a common balanced location. 