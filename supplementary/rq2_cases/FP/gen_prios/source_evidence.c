void mxit_show_message( struct RXMsgData* mx )
{
	char*				pos;
	int					start;
	unsigned int		end;
	int					emo_ofs;
	char				ii[128];
	char				tag[64];
	int*				img_id;
	if ( mx->got_img ) {
		/* search and replace all emoticon tags with proper image tags */
		while ( ( pos = strstr( mx->msg->str, MXIT_II_TAG ) ) != NULL ) {
			start = pos - mx->msg->str;					/* offset at which MXIT_II_TAG starts */
			emo_ofs = start + strlen( MXIT_II_TAG );	/* offset at which EMO's ID starts */
			end = emo_ofs + 1;							/* offset at which MXIT_II_TAG ends */
			while ( ( end < mx->msg->len ) && ( mx->msg->str[end] != '>' ) )
				end++;
			if ( end == mx->msg->len )			/* end of emoticon tag not found */
				break;
			memset( ii, 0x00, sizeof( ii ) );
			memcpy( ii, &mx->msg->str[emo_ofs], end - emo_ofs );
			/* remove inline image tag */
			g_string_erase( mx->msg, start, ( end - start ) + 1 );
			/* find the image entry */
			img_id = (int*) g_hash_table_lookup( mx->session->iimages, ii );
			if ( !img_id ) {
				/* inline image not found, so we will just skip it */
				purple_debug_error( MXIT_PLUGIN_ID, "inline image NOT found (%s)\n", ii );
			}
			else {
				/* insert img tag */
				g_snprintf( tag, sizeof( tag ), "<img id=\"%i\">", *img_id );
				g_string_insert( mx->msg, start, tag );
			}
		}
	}
#ifdef MXIT_DEBUG_MARKUP
	purple_debug_info( MXIT_PLUGIN_ID, "Markup RX (converted): '%s'\n", mx->msg->str );
#endif
	if ( mx->processed ) {
		/* this message has already been taken care of, so just ignore it here */
	}
	else if ( mx->chatid < 0 ) {
		/* normal chat message */
		mxit_show_split_message( mx );
	}
	else {
		/* this is a multimx message */
		serv_got_chat_in( mx->session->con, mx->chatid, mx->from, mx->flags, mx->msg->str, mx->timestamp);
	}
	/* freeup resource */
	free_markupdata( mx );
}
