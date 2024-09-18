John Denbutter
March 27th, 2023

A project that highlights optimal binary search trees for accessing keys in least amount of comparisons.
The optimal binary search tree is compared to the greedy method which favours words with high frequency.

makefile instructions:
	make - creates executable files optimalBST, greedyBST
	make clean - removes the executable files.



optimalBST instructions:
	Description: A program that sorts the words in given file into an Optimal Binary Search Tree.

	Notes:  The given file is expected in the same format as "data.txt".
		Each word in the file can be no longer than 16 characters, since that is the length that will be copied by the program.
			This can be lengthened by modifying the constant: MAX_WORD_LENGTH at the top of the program.
		

	Arguments: No command-line arguments are needed.
		   Upon execution the user will be prompted to enter the search key (EG. "University")
	
	Sample output for successful run:
		$./optimalBST
		Please enter a key:
		where
		Compared with: of (6.147), go right subtree.
		Compared with: the (1.972), go right subtree.
		Compared with: university (0.412), go right subtree.
		Compared with: which (0.132), go left subtree.
		Compared with: variety (0.052), go right subtree.
		Compared with: web (0.022), go right subtree.
		Compared with: week (0.010), go right subtree.
		Compared with: where (0.004), found.

greedyBST instructions:
	Description: A program that sorts the words in given file into a Binary Search Tree, using a greedy method.

	Notes:  The given file is expected in the same format as "data.txt".
		Each word in the file can be no longer than 16 characters, since that is the length that will be copied by the program.
			This can be lengthened by modifying the constant: MAX_WORD_LENGTH at the top of the program.
		

	Arguments: No command-line arguments are needed.
		   Upon execution the user will be prompted to enter the search key (EG. "University")
	
	Sample output for successful run:
		$./greedyBST
		Please enter a key:
		where
		Compared with: the (0.061), go right subtree.
		Compared with: to (0.031), go right subtree.
		Compared with: university (0.018), go right subtree.
		Compared with: with (0.005), go left subtree.
		Compared with: which (0.004), go left subtree.
		Compared with: use (0.003), go right subtree.
		Compared with: variety (0.001), go right subtree.
		Compared with: version (0.001), go right subtree.
		Compared with: web (0.001), go right subtree.
		Compared with: website (0.001), go right subtree.
		Compared with: week (0.001), go right subtree.
		Compared with: where (0.001), found.
