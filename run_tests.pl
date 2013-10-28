#!/usr/bin/perl

use strict;
use warnings;

$|=1;	# Autoflush

print("\nRUNNING TESTS\n");
print("---------------\n");

# Find the tests in this folder
opendir(DIR, "./test/") or die ("Unable to read tests from current directory");
my @tests;

while(my $filename=readdir(DIR)){

	if($filename=~/^test_(\d)$/) {

		push @tests, $1;
	}
}

scalar @tests or die ("No tests found - perhaps you need to 'make tests' first?");

# Run each test
my $completed_tests=0;
for my $test (@tests) {

	print ("-- Test $test\n");
	print `./test/test_$test`;

	if($? != 0) {
	
		print ("************ FAIL\n\n");
		die("Test $test returned nonzero\n");

	}

	$completed_tests++;	
}

# Success
print("**************************\n");
print("* SUCCESS! $completed_tests/".scalar(@tests)." tests OK. *\n");
print("**************************\n");

