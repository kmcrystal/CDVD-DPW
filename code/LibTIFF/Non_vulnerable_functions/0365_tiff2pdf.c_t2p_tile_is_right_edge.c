 */
int t2p_tile_is_right_edge(T2P_TILES tiles, ttile_t tile){
	if( ((tile+1) % tiles.tiles_tilecountx == 0) 
		&& (tiles.tiles_edgetilewidth != 0) ){
		return(1);
	} else {
		return(0);
	}
}
 */
int t2p_tile_is_right_edge(T2P_TILES tiles, ttile_t tile){
	if( ((tile+1) % tiles.tiles_tilecountx == 0) 
		&& (tiles.tiles_edgetilewidth != 0) ){
		return(1);
	} else {
		return(0);
	}
}
