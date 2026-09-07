int t2p_tile_is_corner_edge(T2P_TILES tiles, ttile_t tile){
	return(t2p_tile_is_right_edge(tiles, tile) & t2p_tile_is_bottom_edge(tiles, tile) );
}
