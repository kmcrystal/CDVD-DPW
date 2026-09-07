}
int iax_parse_ies(struct iax_ies *ies, unsigned char *data, int datalen)
{
	/* Parse data into information elements */
	int len;
	int ie;
	char tmp[256], *tmp2;
	struct ast_variable *var, *var2, *prev;
	unsigned int count;
	memset(ies, 0, (int)sizeof(struct iax_ies));
	ies->msgcount = -1;
	ies->firmwarever = -1;
	ies->calling_ton = -1;
	ies->calling_tns = -1;
	ies->calling_pres = -1;
	ies->samprate = IAX_RATE_8KHZ;
	while(datalen >= 2) {
		ie = data[0];
		len = data[1];
		if (len > datalen - 2) {
			errorf("Information element length exceeds message size\n");
			return -1;
		}
		switch(ie) {
		case IAX_IE_CALLED_NUMBER:
			ies->called_number = (char *)data + 2;
			break;
		case IAX_IE_CALLING_NUMBER:
			ies->calling_number = (char *)data + 2;
			break;
		case IAX_IE_CALLING_ANI:
			ies->calling_ani = (char *)data + 2;
			break;
		case IAX_IE_CALLING_NAME:
			ies->calling_name = (char *)data + 2;
			break;
		case IAX_IE_CALLED_CONTEXT:
			ies->called_context = (char *)data + 2;
			break;
		case IAX_IE_USERNAME:
			ies->username = (char *)data + 2;
			break;
		case IAX_IE_PASSWORD:
			ies->password = (char *)data + 2;
			break;
		case IAX_IE_CODEC_PREFS:
			ies->codec_prefs = (char *)data + 2;
			break;
		case IAX_IE_CAPABILITY:
			if (len != (int)sizeof(unsigned int)) {
				snprintf(tmp, (int)sizeof(tmp), "Expecting capability to be %d bytes long but was %d\n", (int)sizeof(unsigned int), len);
				errorf(tmp);
			} else if (ies->capability == 0) { /* Don't overwrite capability2, if specified */
				ies->capability = ntohl(get_unaligned_uint32(data + 2));
			}
			break;
		case IAX_IE_CAPABILITY2:
			{
				int version = data[2];
				if (version == 0) {
					if (len != (int)sizeof(char) + sizeof(iax2_format)) {
						snprintf(tmp, (int)sizeof(tmp), "Expecting capability to be %d bytes long but was %d\n", (int) (sizeof(iax2_format) + sizeof(char)), len);
						errorf(tmp);
					} else {
						ies->capability = (iax2_format) ntohll(get_unaligned_uint64(data + 3));
					}
				} /* else unknown version */
			}
			break;
		case IAX_IE_FORMAT:
			if (len != (int)sizeof(unsigned int)) {
				snprintf(tmp, (int)sizeof(tmp), "Expecting format to be %d bytes long but was %d\n", (int)sizeof(unsigned int), len);
				errorf(tmp);
			} else if (ies->format == 0) { /* Don't overwrite format2, if specified */
				ies->format = ntohl(get_unaligned_uint32(data + 2));
			}
			break;
		case IAX_IE_FORMAT2:
			{
				int version = data[2];
				if (version == 0) {
					if (len != (int)sizeof(char) + sizeof(iax2_format)) {
						snprintf(tmp, (int)sizeof(tmp), "Expecting format to be %d bytes long but was %d\n", (int) (sizeof(iax2_format) + sizeof(char)), len);
						errorf(tmp);
					} else {
						ies->format = (iax2_format) ntohll(get_unaligned_uint64(data + 3));
					}
				} /* else unknown version */
			}
			break;
		case IAX_IE_LANGUAGE:
			ies->language = (char *)data + 2;
			break;
		case IAX_IE_VERSION:
			if (len != (int)sizeof(unsigned short)) {
				snprintf(tmp, (int)sizeof(tmp),  "Expecting version to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
				errorf(tmp);
			} else
				ies->version = ntohs(get_unaligned_uint16(data + 2));
			break;
		case IAX_IE_ADSICPE:
			if (len != (int)sizeof(unsigned short)) {
				snprintf(tmp, (int)sizeof(tmp), "Expecting adsicpe to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
				errorf(tmp);
			} else
				ies->adsicpe = ntohs(get_unaligned_uint16(data + 2));
			break;
		case IAX_IE_SAMPLINGRATE:
			if (len != (int)sizeof(unsigned short)) {
				snprintf(tmp, (int)sizeof(tmp), "Expecting samplingrate to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
				errorf(tmp);
			} else
				ies->samprate = ntohs(get_unaligned_uint16(data + 2));
			break;
		case IAX_IE_DNID:
			ies->dnid = (char *)data + 2;
			break;
		case IAX_IE_RDNIS:
			ies->rdnis = (char *)data + 2;
			break;
		case IAX_IE_AUTHMETHODS:
			if (len != (int)sizeof(unsigned short))  {
				snprintf(tmp, (int)sizeof(tmp), "Expecting authmethods to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
				errorf(tmp);
			} else
				ies->authmethods = ntohs(get_unaligned_uint16(data + 2));
			break;
		case IAX_IE_ENCRYPTION:
			if (len != (int)sizeof(unsigned short))  {
				snprintf(tmp, (int)sizeof(tmp), "Expecting encryption to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
				errorf(tmp);
			} else
				ies->encmethods = ntohs(get_unaligned_uint16(data + 2));
			break;
		case IAX_IE_CHALLENGE:
			ies->challenge = (char *)data + 2;
			break;
		case IAX_IE_MD5_RESULT:
			ies->md5_result = (char *)data + 2;
			break;
		case IAX_IE_RSA_RESULT:
			ies->rsa_result = (char *)data + 2;
			break;
		case IAX_IE_APPARENT_ADDR:
			memcpy(&ies->apparent_addr , (struct ast_sockaddr *) (data + 2), len);
			ies->apparent_addr.len = len;
			break;
		case IAX_IE_REFRESH:
			if (len != (int)sizeof(unsigned short)) {
				snprintf(tmp, (int)sizeof(tmp),  "Expecting refresh to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
				errorf(tmp);
			} else
				ies->refresh = ntohs(get_unaligned_uint16(data + 2));
			break;
		case IAX_IE_DPSTATUS:
			if (len != (int)sizeof(unsigned short)) {
				snprintf(tmp, (int)sizeof(tmp),  "Expecting dpstatus to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
				errorf(tmp);
			} else
				ies->dpstatus = ntohs(get_unaligned_uint16(data + 2));
			break;
		case IAX_IE_CALLNO:
			if (len != (int)sizeof(unsigned short)) {
				snprintf(tmp, (int)sizeof(tmp),  "Expecting callno to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
				errorf(tmp);
			} else
				ies->callno = ntohs(get_unaligned_uint16(data + 2));
			break;
		case IAX_IE_CAUSE:
			ies->cause = (char *)data + 2;
			break;
		case IAX_IE_CAUSECODE:
			if (len != 1) {
				snprintf(tmp, (int)sizeof(tmp), "Expecting causecode to be single byte but was %d\n", len);
				errorf(tmp);
			} else {
				ies->causecode = data[2];
			}
			break;
		case IAX_IE_IAX_UNKNOWN:
			if (len == 1)
				ies->iax_unknown = data[2];
			else {
				snprintf(tmp, (int)sizeof(tmp), "Expected single byte Unknown command, but was %d long\n", len);
				errorf(tmp);
			}
			break;
		case IAX_IE_MSGCOUNT:
			if (len != (int)sizeof(unsigned short)) {
				snprintf(tmp, (int)sizeof(tmp), "Expecting msgcount to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
				errorf(tmp);
			} else
				ies->msgcount = ntohs(get_unaligned_uint16(data + 2));	
			break;
		case IAX_IE_AUTOANSWER:
			ies->autoanswer = 1;
			break;
		case IAX_IE_MUSICONHOLD:
			ies->musiconhold = 1;
			break;
		case IAX_IE_TRANSFERID:
			if (len != (int)sizeof(unsigned int)) {
				snprintf(tmp, (int)sizeof(tmp), "Expecting transferid to be %d bytes long but was %d\n", (int)sizeof(unsigned int), len);
				errorf(tmp);
			} else
				ies->transferid = ntohl(get_unaligned_uint32(data + 2));
			break;
		case IAX_IE_DATETIME:
			if (len != (int)sizeof(unsigned int)) {
				snprintf(tmp, (int)sizeof(tmp), "Expecting date/time to be %d bytes long but was %d\n", (int)sizeof(unsigned int), len);
				errorf(tmp);
			} else
				ies->datetime = ntohl(get_unaligned_uint32(data + 2));
			break;
		case IAX_IE_FIRMWAREVER:
			if (len != (int)sizeof(unsigned short)) {
				snprintf(tmp, (int)sizeof(tmp), "Expecting firmwarever to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
				errorf(tmp);
			} else
				ies->firmwarever = ntohs(get_unaligned_uint16(data + 2));	
			break;
		case IAX_IE_DEVICETYPE:
			ies->devicetype = (char *)data + 2;
			break;
		case IAX_IE_SERVICEIDENT:
			ies->serviceident = (char *)data + 2;
			break;
		case IAX_IE_FWBLOCKDESC:
			if (len != (int)sizeof(unsigned int)) {
				snprintf(tmp, (int)sizeof(tmp), "Expected block desc to be %d bytes long but was %d\n", (int)sizeof(unsigned int), len);
				errorf(tmp);
			} else
				ies->fwdesc = ntohl(get_unaligned_uint32(data + 2));
			break;
		case IAX_IE_FWBLOCKDATA:
			ies->fwdata = data + 2;
			ies->fwdatalen = len;
			break;
		case IAX_IE_ENCKEY:
			ies->enckey = data + 2;
			ies->enckeylen = len;
			break;
		case IAX_IE_PROVVER:
			if (len != (int)sizeof(unsigned int)) {
				snprintf(tmp, (int)sizeof(tmp), "Expected provisioning version to be %d bytes long but was %d\n", (int)sizeof(unsigned int), len);
				errorf(tmp);
			} else {
				ies->provverpres = 1;
				ies->provver = ntohl(get_unaligned_uint32(data + 2));
			}
			break;
		case IAX_IE_CALLINGPRES:
			if (len == 1)
				ies->calling_pres = data[2];
			else {
				snprintf(tmp, (int)sizeof(tmp), "Expected single byte callingpres, but was %d long\n", len);
				errorf(tmp);
			}
			break;
		case IAX_IE_CALLINGTON:
			if (len == 1)
				ies->calling_ton = data[2];
			else {
				snprintf(tmp, (int)sizeof(tmp), "Expected single byte callington, but was %d long\n", len);
				errorf(tmp);
			}
			break;
		case IAX_IE_CALLINGTNS:
			if (len != (int)sizeof(unsigned short)) {
				snprintf(tmp, (int)sizeof(tmp), "Expecting callingtns to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
				errorf(tmp);
			} else
				ies->calling_tns = ntohs(get_unaligned_uint16(data + 2));	
			break;
               case IAX_IE_RR_JITTER:
                       if (len != (int)sizeof(unsigned int)) {
                               snprintf(tmp, (int)sizeof(tmp), "Expected jitter rr to be %d bytes long but was %d\n", (int)sizeof(unsigned int), len);
                               errorf(tmp);
                       } else {
                               ies->rr_jitter = ntohl(get_unaligned_uint32(data + 2));
                       }
                       break;
               case IAX_IE_RR_LOSS:
                       if (len != (int)sizeof(unsigned int)) {
                               snprintf(tmp, (int)sizeof(tmp), "Expected loss rr to be %d bytes long but was %d\n", (int)sizeof(unsigned int), len);
                               errorf(tmp);
                       } else {
                               ies->rr_loss = ntohl(get_unaligned_uint32(data + 2));
                       }
                       break;
               case IAX_IE_RR_PKTS:
                       if (len != (int)sizeof(unsigned int)) {
                               snprintf(tmp, (int)sizeof(tmp), "Expected packets rr to be %d bytes long but was %d\n", (int)sizeof(unsigned int), len);
                               errorf(tmp);
                       } else {
                               ies->rr_pkts = ntohl(get_unaligned_uint32(data + 2));
                       }
                       break;
               case IAX_IE_RR_DELAY:
                       if (len != (int)sizeof(unsigned short)) {
                               snprintf(tmp, (int)sizeof(tmp), "Expected loss rr to be %d bytes long but was %d\n", (int)sizeof(unsigned short), len);
                        errorf(tmp);
                       } else {
                               ies->rr_delay = ntohs(get_unaligned_uint16(data + 2));
                       }
                       break;
		case IAX_IE_RR_DROPPED:
			if (len != (int)sizeof(unsigned int)) {
				snprintf(tmp, (int)sizeof(tmp), "Expected packets rr to be %d bytes long but was %d\n", (int)sizeof(unsigned int), len);
				errorf(tmp);
			} else {
				ies->rr_dropped = ntohl(get_unaligned_uint32(data + 2));
			}
			break;
		case IAX_IE_RR_OOO:
			if (len != (int)sizeof(unsigned int)) {
				snprintf(tmp, (int)sizeof(tmp), "Expected packets rr to be %d bytes long but was %d\n", (int)sizeof(unsigned int), len);
				errorf(tmp);
			} else {
				ies->rr_ooo = ntohl(get_unaligned_uint32(data + 2));
			}
			break;
		case IAX_IE_VARIABLE:
			ast_copy_string(tmp, (char *)data + 2, len + 1);
			tmp2 = strchr(tmp, '=');
			if (tmp2)
				*tmp2++ = '\0';
			else
				tmp2 = "";
			{
				struct ast_str *str = ast_str_create(16);
				/* Existing variable or new variable? */
				for (var2 = ies->vars, prev = NULL; var2; prev = var2, var2 = var2->next) {
					if (strcmp(tmp, var2->name) == 0) {
						ast_str_set(&str, 0, "%s%s", var2->value, tmp2);
						var = ast_variable_new(tmp, ast_str_buffer(str), var2->file);
						var->next = var2->next;
						if (prev) {
							prev->next = var;
						} else {
							ies->vars = var;
						}
						snprintf(tmp, sizeof(tmp), "Assigned (%p)%s to (%p)%s\n", var->name, var->name, var->value, var->value);
						outputf(tmp);
						ast_free(var2);
						break;
					}
				}
				ast_free(str);
			}
			if (!var2) {
				var = ast_variable_new(tmp, tmp2, "");
				snprintf(tmp, sizeof(tmp), "Assigned (%p)%s to (%p)%s\n", var->name, var->name, var->value, var->value);
				outputf(tmp);
				var->next = ies->vars;
				ies->vars = var;
			}
			break;
		case IAX_IE_OSPTOKEN:
			if ((count = data[2]) < IAX_MAX_OSPBLOCK_NUM) {
				ies->osptokenblock[count] = (char *)data + 2 + 1;
				ies->ospblocklength[count] = len - 1;
			} else {
				snprintf(tmp, (int)sizeof(tmp), "Expected OSP token block index to be 0~%d but was %u\n", IAX_MAX_OSPBLOCK_NUM - 1, count);
				errorf(tmp);
			}
			break;
		case IAX_IE_CALLTOKEN:
			if (len) {
				ies->calltokendata = (unsigned char *) data + 2;
			}
			ies->calltoken = 1;
			break;
		default:
			snprintf(tmp, (int)sizeof(tmp), "Ignoring unknown information element '%s' (%d) of length %d\n", iax_ie2str(ie), ie, len);
			outputf(tmp);
		}
		/* Overwrite information element with 0, to null terminate previous portion */
		data[0] = 0;
		datalen -= (len + 2);
		data += (len + 2);
	}
	/* Null-terminate last field */
	*data = '\0';
	if (datalen) {
		errorf("Invalid information element contents, strange boundary\n");
		return -1;
	}
	return 0;
}
