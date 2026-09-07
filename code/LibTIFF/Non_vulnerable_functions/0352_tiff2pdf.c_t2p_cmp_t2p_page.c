 */
int t2p_cmp_t2p_page(const void* e1, const void* e2){
	int d;
	d = (int32)(((T2P_PAGE*)e1)->page_number) - (int32)(((T2P_PAGE*)e2)->page_number);
	if(d == 0){
		d = (int32)(((T2P_PAGE*)e1)->page_directory) - (int32)(((T2P_PAGE*)e2)->page_directory);
	}
	return d;
}
