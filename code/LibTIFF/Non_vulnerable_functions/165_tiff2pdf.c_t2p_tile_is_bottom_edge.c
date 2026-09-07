 */
int t2p_tile_is_bottom_edge(T2P_TILES tiles, ttile_t tile){
	if( ((tile+1) > (tiles.tiles_tilecount-tiles.tiles_tilecountx) )
		&& (tiles.tiles_edgetilelength != 0) ){
		return(1);
	} else {
		return(0);
	}
}
 */
int t2p_tile_is_bottom_edge(T2P_TILES tiles, ttile_t tile){
	if( ((tile+1) > (tiles.tiles_tilecount-tiles.tiles_tilecountx) )
		&& (tiles.tiles_edgetilelength != 0) ){
		return(1);
	} else {
		return(0);
	}
}
