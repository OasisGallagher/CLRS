/**
Question 7. Water Jugs: CLRS 8-4 (2nd edition pg. 179).
Solution:
Part a) A simple deterministic algorithm to group the jugs in pairs is the following. Select a jug
from the blue set. Compare it with every red jug and find its pair. Do this for all the blue jugs.
Clearly, the algorithm will find the correct grouping, but the total number of comparisons
performed by it is ¡¦(n
2
).
Part b) Consider the red jugs in some arbitrary order. The correct matching will identify one
permutation of blue jugs out of n! possible permutations of blue jugs. A comparison tree that does
this will have >= n! leaves, and hence height (n lg n).
Part c) Consider the following algorithm for grouping the jugs into pairs, which works much like
the randomized version of quicksort. Select a random blue jug and partition the red jugs with it into
two groups: one group that contains red jugs that are smaller than the selected blue jug, and one
group that contains red jugs that are larger than the selected blue jug. While doing the partitioning
find the red jug which is equal to the selected blue jug. Now, using the red jug, which is equal to
the selected blue jug, partition the blue jugs. So we have obtained 2 sets of partitions that match up.
Now, recursively perform the same operations on these new partitions of jugs until the size of the
partitions > 1. If during the execution of this algorithm we make sure that the partition, which
contains jugs that are smaller than the selected jug is placed to the left of the selected jug, while the
other partition is placed to the right the algorithms will not only pair the jugs but will also sort
them. Since this algorithm closely reassembles quicksort the same running time analysis applies to
it. Thus the expected running time of this algorithm will be O(n lg n), while the worst case running
time will be O(n
2
).
*/
