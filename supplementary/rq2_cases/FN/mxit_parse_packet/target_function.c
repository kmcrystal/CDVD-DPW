int mxit_parse_packet( struct MXitSession* session )
{
	struct rx_packet	packet;
	struct record*		rec;
	struct field*		field;
	gboolean			pbreak;
	unsigned int		i;
	int					res	= 0;
#ifdef	DEBUG_PROTOCOL
	purple_debug_info( MXIT_PLUGIN_ID, "Received packet (%i bytes)\n", session->rx_i );
	dump_bytes( session, session->rx_dbuf, session->rx_i );
#endif
	i = 0;
	while ( i < session->rx_i ) {
		/* create first record and field */
		rec = NULL;
		field = NULL;
		memset( &packet, 0x00, sizeof( struct rx_packet ) );
		rec = add_record( &packet );
		pbreak = FALSE;
		/* break up the received packet into fields and records for easy parsing */
		while ( ( i < session->rx_i ) && ( !pbreak ) ) {
			switch ( session->rx_dbuf[i] ) {
				case CP_SOCK_REC_TERM :
						/* new record */
						if ( packet.rcount == 1 ) {
							/* packet command */
							packet.cmd = atoi( packet.records[0]->fields[0]->data );
						}
						else if ( packet.rcount == 2 ) {
							/* special case: binary multimedia packets should not be parsed here */
							if ( packet.cmd == CP_CMD_MEDIA ) {
								/* add the chunked to new record */
								rec = add_record( &packet );
								field = add_field( rec );
								field->data = &session->rx_dbuf[i + 1];
								field->len = session->rx_i - i;
								/* now skip the binary data */
								res = get_chunk_len( field->data );
								/* determine if we have more packets */
								if ( res + 6 + i < session->rx_i ) {
									/* we have more than one packet in this stream */
									i += res + 6;
									pbreak = TRUE;
								}
								else {
									i = session->rx_i;
								}
							}
						}
						else if ( !field ) {
							field = add_field( rec );
							field->data = &session->rx_dbuf[i];
						}
						session->rx_dbuf[i] = '\0';
						rec = add_record( &packet );
						field = NULL;
						break;
				case CP_FLD_TERM :
						/* new field */
						session->rx_dbuf[i] = '\0';
						if ( !field ) {
							field = add_field( rec );
							field->data = &session->rx_dbuf[i];
						}
						field = NULL;
						break;
				case CP_PKT_TERM :
						/* packet is done! */
						session->rx_dbuf[i] = '\0';
						pbreak = TRUE;
						break;
				default :
						/* skip non special characters */
						if ( !field ) {
							field = add_field( rec );
							field->data = &session->rx_dbuf[i];
						}
						field->len++;
						break;
			}
			i++;
		}
		if ( packet.rcount < 2 ) {
			/* bad packet */
			purple_connection_error( session->con, _( "Invalid packet received from MXit." ) );
			free_rx_packet( &packet );
			continue;
		}
		session->rx_dbuf[session->rx_i] = '\0';
		packet.errcode = atoi( packet.records[1]->fields[0]->data );
		purple_debug_info( MXIT_PLUGIN_ID, "Packet received CMD:%i (%i)\n", packet.cmd, packet.errcode );
#ifdef	DEBUG_PROTOCOL
		/* debug */
		dump_packet( &packet );
#endif
		/* reset the out ack */
		if ( session->outack == packet.cmd ) {
			/* outstanding ack received from mxit server */
			session->outack = 0;
		}
		/* check packet status */
		if ( packet.errcode != MXIT_ERRCODE_SUCCESS ) {
			/* error reply! */
			if ( ( packet.records[1]->fcount > 1 ) && ( packet.records[1]->fields[1]->data ) )
				packet.errmsg = packet.records[1]->fields[1]->data;
			else
				packet.errmsg = NULL;
			res = process_error_response( session, &packet );
		}
		else {
			/* success reply! */
			res = process_success_response( session, &packet );
		}
		/* free up the packet resources */
		free_rx_packet( &packet );
	}
	if ( session->outack == 0 )
			mxit_manage_queue( session );
	return res;
}
