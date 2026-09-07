static unsigned char cluster_128[] = { 44, 40, 63, 59, 230, 95 };
static int check_cluster( int cluster, unsigned char *buffer, unsigned char *expected_cluster ) {
	unsigned char *target = buffer + cluster*6;
	if (memcmp(target, expected_cluster, 6) == 0) {
		return 0;
	}
	fprintf( stderr, "Cluster %d did not match expected results.\n", cluster );
	fprintf( stderr, 
		 "Expect: %3d %3d   %3d   %3d\n"
		 "        %3d %3d\n", 
		 expected_cluster[0], expected_cluster[1],
		 expected_cluster[4], expected_cluster[5],
		 expected_cluster[2], expected_cluster[3] );
	fprintf( stderr, 
		 "   Got: %3d %3d   %3d   %3d\n"
		 "        %3d %3d\n", 
		 target[0], target[1], 
		 target[4], target[5],
		 target[2], target[3] );
	return 1;
}
