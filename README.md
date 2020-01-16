# Bioinformatics

This repository hosts an implementation of a **minimizer** bioinformatics algorithm.

The code was developed accordingly to the paper:
[*"Reducing storage requirements for biological sequence comparison"*](https://academic.oup.com/bioinformatics/article/20/18/3363/202143) by Michael Roberts, Wayne Hayes, Brian R. Hunt, Stephen M. Mount and James A. Yorke.

Many bioinformatics algorithms use **short substrings of a longer sequence**, commonly known as ***k*-mers**, for indexing, search or assembly. **Minimizers** allow **efficient binning of those *k*-mers** so that some information about the sequence contiguity is preserved.

## How to compile and run

Compiled with g++ version 9.2.0:
```
g++ minimizer.cpp -o minimize
```

Run with:
```
./minimize INPUT_FILE OUTPUT_FILE W K F
```

with **W, K and F** being:
- **W**: window size in k-mers (how many k-mers per window)
- **K**: k-mer size
- **F**: percentage of top most frequent minimizers to remove (0 to 1).

## **Review of *"Reducing storage requirements for biological sequence comparison"***

Sequence comparison is common in bioinformatics, used for example in applications such as overlap determination in genome sequences and genome assembly algorithms.

One commonly used method is the **seed and extend** approach. The first step is **choosing the set of *seeds*** that represent each string. ***Seeds* are contiguous *k*-letter substrings called *k*-mers. **

The number of *k*-mer entries and the space required to store the entire list of *k*-mers can be too large. Hence, to **reduce the storage space** required we must **store less *k*-mers**. But **which ones** to choose?

Thus, the authors propose a method that allows us to select from each string a **set of special *k*-mers (to be used as seeds) called minimizers.**

### **Interior minimizers**

The first step in choosing minimizers, is to **select an ordering** for the set of all *k*-mers. **One convenient ordering is simply lexicographic order** (with AAAA being the “smallest” possible 4-mer). 

Furthermore, the authors point out that *w* adjacent *k*-mers correspond to a window of *l = w + k − 1* letters. Essentially, a set of *w* consecutive *k*-mers covers a string of exactly *w + k − 1* letters. In this context, *consecutive* means that each *k*-mer is shifted by one letter from the previous one.

**After ordering**, examine *w* consecutive *k*-mers and **select the smallest as the window minimizer.**

Additionally, adjacent windows often share the same minimizer. Consequently, **gaps between minimizers can appear** and are caused when the minimizers of two adjacent windows are more than *k* positions apart. Gaps can be at most *w − k* in size, so **setting *w ≤ k* ensures no gaps occur between minimizers.** On the other hand, if *w > k*, minimizers are sparse in the string.


### **End minimizers (left-end and right-end)**

As mentioned, *w ≤ k* guarantees that no gaps appear between adjacent minimizers, but it still allows **some letters at each end of the string to be outside any minimizers.** If the match is less than *w + k − 1* letters, then it is possible for the strings to have no *(w, k)*-minimizer in common. **This problem is easily fixed by the introduction of end-minimizers**. A *(u, k)*-end-minimizer is chosen from a window of size *u* which is anchored to one end of the string, and the set of *k*-end-minimizers are comprised of all such *(u, k)*-end-minimizers for *u* from 1 up to some maximum window size *v*.

### **A mixed strategy**
Finally, **combining both *(w, k)*-minimizers of a string with *(u, k)*-end-minimizers** for *u = 1, . . . , w−1* at both ends of the string, if *w ≤ k*, **every base in a string will be covered with some minimizer.**

After finding the minimizers for the given genome, it is a good practice to **remove the most frequent ones**, since these are not good for sequence matching (because **their repitition across the genome means it is harder to conclude where that substring came from when aligning two strings**).
