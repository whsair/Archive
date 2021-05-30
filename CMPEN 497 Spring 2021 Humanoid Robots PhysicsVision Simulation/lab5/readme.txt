version: 2.83.10

1. 4 dof in this robot arm.
Bone3 (The rod connecting the robot's hand with furthest joint with world origin) rotation in z-axis; 
Bone2 (The longest rod) location in y-direction;
Bone1 (The shortest rod) rotation in y-axis;
Bone (the rob along in y = 0 and x = 0 direction) rotation in y-axis.

2. 6 dof since 4 original dof + 2 (the base can move in x and y direction)

3. forward kinematics chain is using calculating forward by using the array of
joints information. We are provided with free parameters for each joint.

Inverse kinematics chain, unlike forward chain, is not unique. We are given with the final location of
our object without knowing any parameter of joints. It's possible to have different inverse chain for
the same given final location.

4. Yes, since forward chain are made up with the array of unique links and can be represented by the product
of several transformation matrixes correponding each joint. each transformation is unqiue and the order of
the product of matrix does matter.

5. No, it doesn't guarantee that you will have only configuration of the robot. The lecture has already provide lots
of counter examples. We simply fix cube location then change third bone location to get another answer.
In our system, the degree of freedom of our cube is just 3 which is less than dof of our robot arm.
That means when we are solving the homogenous system there will be 4 - 3 = 1 free variable.

6. No. Due to the limit amount of dof, The arm cannot reach any point along x = 0 and y = 0
which is the z-axis in world coordinate.