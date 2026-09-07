}
static int gen_prios(struct ael_extension *exten, char *label, pval *statement, struct ael_extension *mother_exten, struct ast_context *this_context )
{
	pval *p,*p2,*p3;
	struct ael_priority *pr;
	struct ael_priority *for_init, *for_test, *for_inc, *for_loop, *for_end;
	struct ael_priority *while_test, *while_loop, *while_end;
	struct ael_priority *switch_set, *switch_test, *switch_end, *fall_thru, *switch_empty;
	struct ael_priority *if_test, *if_end, *if_skip, *if_false;
#ifdef OLD_RAND_ACTION
	struct ael_priority *rand_test, *rand_end, *rand_skip;
#endif
	RAII_VAR(char *, buf1, NULL, free);
	RAII_VAR(char *, buf2, NULL, free);
	RAII_VAR(char *, new_label, NULL, free);
	char *strp, *strp2;
	int default_exists;
	int local_control_statement_count;
	int first;
	struct ael_priority *loop_break_save;
	struct ael_priority *loop_continue_save;
	struct ael_extension *switch_case,*switch_null;
	if (!(buf1 = malloc(BUF_SIZE))) {
		return -1;
	}
	if (!(buf2 = malloc(BUF_SIZE))) {
		return -1;
	}
	if (!(new_label = malloc(BUF_SIZE))) {
		return -1;
	}
	if ((mother_exten && !mother_exten->checked_switch) || (exten && !exten->checked_switch)) {
		if (contains_switch(statement)) { /* only run contains_switch if you haven't checked before */
			if (mother_exten) {
				if (!mother_exten->has_switch) {
					for (first = 1; first >= 0; first--) {
						switch_set = new_prio();
						switch_set->type = AEL_APPCALL;
						switch_set->app = strdup("MSet");
						/* Are we likely inside a gosub subroutine? */
						if (!strcmp(mother_exten->name, "~~s~~") && first) {
							/* If we're not actually within a gosub, this will fail, but the
							 * second time through, it will get set.  If we are within gosub,
							 * the second time through is redundant, but acceptable. */
							switch_set->appargs = strdup("LOCAL(~~EXTEN~~)=${EXTEN}");
						} else {
							switch_set->appargs = strdup("~~EXTEN~~=${EXTEN}");
							first = 0;
						}
						linkprio(exten, switch_set, mother_exten);
						mother_exten->has_switch = 1;
						mother_exten->checked_switch = 1;
						if (exten) {
							exten->has_switch = 1;
							exten->checked_switch = 1;
						}
					}
				}
			} else if (exten) {
				if (!exten->has_switch) {
					for (first = 1; first >= 0; first--) {
						switch_set = new_prio();
						switch_set->type = AEL_APPCALL;
						switch_set->app = strdup("MSet");
						/* Are we likely inside a gosub subroutine? */
						if (!strcmp(exten->name, "~~s~~")) {
							/* If we're not actually within a gosub, this will fail, but the
							 * second time through, it will get set.  If we are within gosub,
							 * the second time through is redundant, but acceptable. */
							switch_set->appargs = strdup("LOCAL(~~EXTEN~~)=${EXTEN}");
						} else {
							switch_set->appargs = strdup("~~EXTEN~~=${EXTEN}");
							first = 0;
						}
						linkprio(exten, switch_set, mother_exten);
						exten->has_switch = 1;
						exten->checked_switch = 1;
						if (mother_exten) {
							mother_exten->has_switch = 1;
							mother_exten->checked_switch = 1;
						}
					}
				}
			}
		} else {
			if (mother_exten) {
				mother_exten->checked_switch = 1;
			}
			if (exten) {
				exten->checked_switch = 1;
			}
		}
	}
	for (p=statement; p; p=p->next) {
		switch (p->type) {
		case PV_VARDEC:
			pr = new_prio();
			pr->type = AEL_APPCALL;
			snprintf(buf1, BUF_SIZE, "%s=$[%s]", p->u1.str, p->u2.val);
			pr->app = strdup("MSet");
			remove_spaces_before_equals(buf1);
			pr->appargs = strdup(buf1);
			pr->origin = p;
			linkprio(exten, pr, mother_exten);
			break;
		case PV_LOCALVARDEC:
			pr = new_prio();
			pr->type = AEL_APPCALL;
			snprintf(buf1, BUF_SIZE, "LOCAL(%s)=$[%s]", p->u1.str, p->u2.val);
			pr->app = strdup("MSet");
			remove_spaces_before_equals(buf1);
			pr->appargs = strdup(buf1);
			pr->origin = p;
			linkprio(exten, pr, mother_exten);
			break;
		case PV_GOTO:
			pr = new_prio();
			pr->type = AEL_APPCALL;
			p->u2.goto_target = get_goto_target(p);
			if( p->u2.goto_target ) {
				p->u3.goto_target_in_case = label_inside_case(p->u2.goto_target);
			}
			if (!p->u1.list->next) /* just one */ {
				pr->app = strdup("Goto");
				if (!mother_exten)
					pr->appargs = strdup(p->u1.list->u1.str);
				else {  /* for the case of simple within-extension gotos in case/pattern/default statement blocks: */ 
					snprintf(buf1, BUF_SIZE, "%s,%s", mother_exten->name, p->u1.list->u1.str);
					pr->appargs = strdup(buf1);
				}
			} else if (p->u1.list->next && !p->u1.list->next->next) /* two */ {
				snprintf(buf1, BUF_SIZE, "%s,%s", p->u1.list->u1.str, p->u1.list->next->u1.str);
				pr->app = strdup("Goto");
				pr->appargs = strdup(buf1);
			} else if (p->u1.list->next && p->u1.list->next->next) {
				snprintf(buf1, BUF_SIZE, "%s,%s,%s", p->u1.list->u1.str, 
						p->u1.list->next->u1.str,
						p->u1.list->next->next->u1.str);
				pr->app = strdup("Goto");
				pr->appargs = strdup(buf1);
			}
			pr->origin = p;
			linkprio(exten, pr, mother_exten);
			break;
		case PV_LABEL:
			pr = new_prio();
			pr->type = AEL_LABEL;
			pr->origin = p;
			p->u3.compiled_label = exten;
			linkprio(exten, pr, mother_exten);
			break;
		case PV_FOR:
			control_statement_count++;
			loop_break_save = exten->loop_break; /* save them, then restore before leaving */
			loop_continue_save = exten->loop_continue;
			snprintf(new_label, BUF_SIZE, "for_%s_%d", label, control_statement_count);
			for_init = new_prio();
			for_inc = new_prio();
			for_test = new_prio();
			for_loop = new_prio();
			for_end = new_prio();
			for_init->type = AEL_APPCALL;
			for_inc->type = AEL_APPCALL;
			for_test->type = AEL_FOR_CONTROL;
			for_test->goto_false = for_end;
			for_loop->type = AEL_CONTROL1; /* simple goto */
			for_end->type = AEL_APPCALL;
			for_init->app = strdup("MSet");
			strcpy(buf2,p->u1.for_init);
			remove_spaces_before_equals(buf2);
			strp = strchr(buf2, '=');
			if (strp) {
				strp2 = strchr(p->u1.for_init, '=');
				*(strp+1) = 0;
				strcat(buf2,"$[");
				strncat(buf2,strp2+1, BUF_SIZE-strlen(strp2+1)-2);
				strcat(buf2,"]");
				for_init->appargs = strdup(buf2);
			} else {
				strp2 = p->u1.for_init;
				while (*strp2 && isspace(*strp2))
					strp2++;
				if (*strp2 == '&') { /* itsa macro call */
					char *strp3 = strp2+1;
					while (*strp3 && isspace(*strp3))
						strp3++;
					strcpy(buf2, strp3);
					strp3 = strchr(buf2,'(');
					if (strp3) {
						*strp3 = '|';
					}
					while ((strp3=strchr(buf2,','))) {
						*strp3 = '|';
					}
					strp3 = strrchr(buf2, ')');
					if (strp3)
						*strp3 = 0; /* remove the closing paren */
					for_init->appargs = strdup(buf2);
					free(for_init->app);
					for_init->app = strdup("Macro");
				} else {  /* must be a regular app call */
					char *strp3;
					strcpy(buf2, strp2);
					strp3 = strchr(buf2,'(');
					if (strp3) {
						*strp3 = 0;
						free(for_init->app);
						for_init->app = strdup(buf2);
						for_init->appargs = strdup(strp3+1);
						strp3 = strrchr(for_init->appargs, ')');
						if (strp3)
							*strp3 = 0; /* remove the closing paren */
					}
				}
			}
			strcpy(buf2,p->u3.for_inc);
			remove_spaces_before_equals(buf2);
			strp = strchr(buf2, '=');
			if (strp) {  /* there's an = in this part; that means an assignment. set it up */
				strp2 = strchr(p->u3.for_inc, '=');
				*(strp+1) = 0;
				strcat(buf2,"$[");
				strncat(buf2,strp2+1, BUF_SIZE-strlen(strp2+1)-2);
				strcat(buf2,"]");
				for_inc->appargs = strdup(buf2);
				for_inc->app = strdup("MSet");
			} else {
				strp2 = p->u3.for_inc;
				while (*strp2 && isspace(*strp2))
					strp2++;
				if (*strp2 == '&') { /* itsa macro call  */
					char *strp3 = strp2+1;
					while (*strp3 && isspace(*strp3))
						strp3++;
					strcpy(buf2, strp3);
					strp3 = strchr(buf2,'(');
					if (strp3) {
						*strp3 = ',';
					}
					strp3 = strrchr(buf2, ')');
					if (strp3)
						*strp3 = 0; /* remove the closing paren */
					for_inc->appargs = strdup(buf2);
					for_inc->app = strdup("Macro");
				} else {  /* must be a regular app call */
					char *strp3;
					strcpy(buf2, strp2);
					strp3 = strchr(buf2,'(');
					if (strp3) {
						*strp3 = 0;
						for_inc->app = strdup(buf2);
						for_inc->appargs = strdup(strp3+1);
						strp3 = strrchr(for_inc->appargs, ')');
						if (strp3)
							*strp3 = 0; /* remove the closing paren */
					}
				}
			}
			snprintf(buf1, BUF_SIZE, "$[%s]",p->u2.for_test);
			for_test->app = 0;
			for_test->appargs = strdup(buf1);
			for_loop->goto_true = for_test;
			snprintf(buf1, BUF_SIZE, "Finish for_%s_%d", label, control_statement_count);
			for_end->app = strdup("NoOp");
			for_end->appargs = strdup(buf1);
			/* link & load! */
			linkprio(exten, for_init, mother_exten);
			linkprio(exten, for_test, mother_exten);
			/* now, put the body of the for loop here */
			exten->loop_break = for_end;
			exten->loop_continue = for_inc;
			if (gen_prios(exten, new_label, p->u4.for_statements, mother_exten, this_context)) { /* this will link in all the statements here */
				return -1;
			}
			linkprio(exten, for_inc, mother_exten);
			linkprio(exten, for_loop, mother_exten);
			linkprio(exten, for_end, mother_exten);
			exten->loop_break = loop_break_save;
			exten->loop_continue = loop_continue_save;
			for_loop->origin = p;
			break;
		case PV_WHILE:
			control_statement_count++;
			loop_break_save = exten->loop_break; /* save them, then restore before leaving */
			loop_continue_save = exten->loop_continue;
			snprintf(new_label, BUF_SIZE, "while_%s_%d", label, control_statement_count);
			while_test = new_prio();
			while_loop = new_prio();
			while_end = new_prio();
			while_test->type = AEL_FOR_CONTROL;
			while_test->goto_false = while_end;
			while_loop->type = AEL_CONTROL1; /* simple goto */
			while_end->type = AEL_APPCALL;
			snprintf(buf1, BUF_SIZE, "$[%s]",p->u1.str);
			while_test->app = 0;
			while_test->appargs = strdup(buf1);
			while_loop->goto_true = while_test;
			snprintf(buf1, BUF_SIZE, "Finish while_%s_%d", label, control_statement_count);
			while_end->app = strdup("NoOp");
			while_end->appargs = strdup(buf1);
			linkprio(exten, while_test, mother_exten);
			/* now, put the body of the for loop here */
			exten->loop_break = while_end;
			exten->loop_continue = while_test;
			if (gen_prios(exten, new_label, p->u2.statements, mother_exten, this_context)) { /* this will link in all the while body statements here */
				return -1;
			}
			linkprio(exten, while_loop, mother_exten);
			linkprio(exten, while_end, mother_exten);
			exten->loop_break = loop_break_save;
			exten->loop_continue = loop_continue_save;
			while_loop->origin = p;
			break;
		case PV_SWITCH:
			control_statement_count++;
			local_control_statement_count = control_statement_count;
			loop_break_save = exten->loop_break; /* save them, then restore before leaving */
			loop_continue_save = exten->loop_continue;
			snprintf(new_label, BUF_SIZE, "sw_%s_%d", label, control_statement_count);
			switch_test = new_prio();
			switch_end = new_prio();
			switch_test->type = AEL_APPCALL;
			switch_end->type = AEL_APPCALL;
			snprintf(buf1, BUF_SIZE, "sw_%d_%s,10", control_statement_count, p->u1.str);
			switch_test->app = strdup("Goto");
			switch_test->appargs = strdup(buf1);
			snprintf(buf1, BUF_SIZE, "Finish switch_%s_%d", label, control_statement_count);
			switch_end->app = strdup("NoOp");
			switch_end->appargs = strdup(buf1);
			switch_end->origin = p;
			switch_end->exten = exten;
			linkprio(exten, switch_test, mother_exten);
			linkprio(exten, switch_end, mother_exten);
			exten->loop_break = switch_end;
			exten->loop_continue = 0;
			default_exists = 0;
			for (p2=p->u2.statements; p2; p2=p2->next) {
				/* now, for each case/default put the body of the for loop here */
				if (p2->type == PV_CASE) {
					/* ok, generate a extension and link it in */
					switch_case = new_exten();
					if (mother_exten && mother_exten->checked_switch) {
						switch_case->has_switch = mother_exten->has_switch;
						switch_case->checked_switch = mother_exten->checked_switch;
					}
					if (exten && exten->checked_switch) {
						switch_case->has_switch = exten->has_switch;
						switch_case->checked_switch = exten->checked_switch;
					}
					switch_case->context = this_context;
					switch_case->is_switch = 1;
					/* the break/continue locations are inherited from parent */
					switch_case->loop_break = exten->loop_break;
					switch_case->loop_continue = exten->loop_continue;
					linkexten(exten,switch_case);
					snprintf(buf1, BUF_SIZE, "sw_%d_%s", local_control_statement_count, p2->u1.str);
					switch_case->name = strdup(buf1);
					snprintf(new_label, BUF_SIZE, "sw_%s_%s_%d", label, p2->u1.str, local_control_statement_count);
					if (gen_prios(switch_case, new_label, p2->u2.statements, exten, this_context)) { /* this will link in all the case body statements here */
						return -1;
					}
					/* here is where we write code to "fall thru" to the next case... if there is one... */
					for (p3=p2->u2.statements; p3; p3=p3->next) {
						if (!p3->next)
							break;
					}
					/* p3 now points the last statement... */
					if (!p3 || ( p3->type != PV_GOTO && p3->type != PV_BREAK && p3->type != PV_RETURN) ) {
						/* is there a following CASE/PATTERN/DEFAULT? */
						if (p2->next && p2->next->type == PV_CASE) {
							fall_thru = new_prio();
							fall_thru->type = AEL_APPCALL;
							fall_thru->app = strdup("Goto");
							snprintf(buf1, BUF_SIZE, "sw_%d_%s,10", local_control_statement_count, p2->next->u1.str);
							fall_thru->appargs = strdup(buf1);
							linkprio(switch_case, fall_thru, mother_exten);
						} else if (p2->next && p2->next->type == PV_PATTERN) {
							fall_thru = new_prio();
							fall_thru->type = AEL_APPCALL;
							fall_thru->app = strdup("Goto");
							gen_match_to_pattern(p2->next->u1.str, buf2);
							snprintf(buf1, BUF_SIZE, "sw_%d_%s,10", local_control_statement_count, buf2);
							fall_thru->appargs = strdup(buf1);
							linkprio(switch_case, fall_thru, mother_exten);
						} else if (p2->next && p2->next->type == PV_DEFAULT) {
							fall_thru = new_prio();
							fall_thru->type = AEL_APPCALL;
							fall_thru->app = strdup("Goto");
							snprintf(buf1, BUF_SIZE, "sw_%d_.,10", local_control_statement_count);
							fall_thru->appargs = strdup(buf1);
							linkprio(switch_case, fall_thru, mother_exten);
						} else if (!p2->next) {
							fall_thru = new_prio();
							fall_thru->type = AEL_CONTROL1;
							fall_thru->goto_true = switch_end;
							fall_thru->app = strdup("Goto");
							linkprio(switch_case, fall_thru, mother_exten);
						}
					}
					if (switch_case->return_needed) { /* returns don't generate a goto eoe (end of extension) any more, just a Return() app call) */
						char buf[2000];
						struct ael_priority *np2 = new_prio();
						np2->type = AEL_APPCALL;
						np2->app = strdup("NoOp");
						snprintf(buf, BUF_SIZE, "End of Extension %s", switch_case->name);
						np2->appargs = strdup(buf);
						linkprio(switch_case, np2, mother_exten);
						switch_case-> return_target = np2;
					}
				} else if (p2->type == PV_PATTERN) {
					/* ok, generate a extension and link it in */
					switch_case = new_exten();
					if (mother_exten && mother_exten->checked_switch) {
						switch_case->has_switch = mother_exten->has_switch;
						switch_case->checked_switch = mother_exten->checked_switch;
					}
					if (exten && exten->checked_switch) {
						switch_case->has_switch = exten->has_switch;
						switch_case->checked_switch = exten->checked_switch;
					}
					switch_case->context = this_context;
					switch_case->is_switch = 1;
					/* the break/continue locations are inherited from parent */
					switch_case->loop_break = exten->loop_break;
					switch_case->loop_continue = exten->loop_continue;
					linkexten(exten,switch_case);
					snprintf(buf1, BUF_SIZE, "_sw_%d_%s", local_control_statement_count, p2->u1.str);
					switch_case->name = strdup(buf1);
					snprintf(new_label, BUF_SIZE, "sw_%s_%s_%d", label, p2->u1.str, local_control_statement_count);
					if (gen_prios(switch_case, new_label, p2->u2.statements, exten, this_context)) { /* this will link in all the while body statements here */
						return -1;
					}
					/* here is where we write code to "fall thru" to the next case... if there is one... */
					for (p3=p2->u2.statements; p3; p3=p3->next) {
						if (!p3->next)
							break;
					}
					/* p3 now points the last statement... */
					if (!p3 || ( p3->type != PV_GOTO && p3->type != PV_BREAK && p3->type != PV_RETURN)) {
						/* is there a following CASE/PATTERN/DEFAULT? */
						if (p2->next && p2->next->type == PV_CASE) {
							fall_thru = new_prio();
							fall_thru->type = AEL_APPCALL;
							fall_thru->app = strdup("Goto");
							snprintf(buf1, BUF_SIZE, "sw_%d_%s,10", local_control_statement_count, p2->next->u1.str);
							fall_thru->appargs = strdup(buf1);
							linkprio(switch_case, fall_thru, mother_exten);
						} else if (p2->next && p2->next->type == PV_PATTERN) {
							fall_thru = new_prio();
							fall_thru->type = AEL_APPCALL;
							fall_thru->app = strdup("Goto");
							gen_match_to_pattern(p2->next->u1.str, buf2);
							snprintf(buf1, BUF_SIZE, "sw_%d_%s,10", local_control_statement_count, buf2);
							fall_thru->appargs = strdup(buf1);
							linkprio(switch_case, fall_thru, mother_exten);
						} else if (p2->next && p2->next->type == PV_DEFAULT) {
							fall_thru = new_prio();
							fall_thru->type = AEL_APPCALL;
							fall_thru->app = strdup("Goto");
							snprintf(buf1, BUF_SIZE, "sw_%d_.,10", local_control_statement_count);
							fall_thru->appargs = strdup(buf1);
							linkprio(switch_case, fall_thru, mother_exten);
						} else if (!p2->next) {
							fall_thru = new_prio();
							fall_thru->type = AEL_CONTROL1;
							fall_thru->goto_true = switch_end;
							fall_thru->app = strdup("Goto");
							linkprio(switch_case, fall_thru, mother_exten);
						}
					}
					if (switch_case->return_needed) { /* returns don't generate a goto eoe (end of extension) any more, just a Return() app call) */
						char buf[2000];
						struct ael_priority *np2 = new_prio();
						np2->type = AEL_APPCALL;
						np2->app = strdup("NoOp");
						snprintf(buf,sizeof(buf),"End of Extension %s", switch_case->name);
						np2->appargs = strdup(buf);
						linkprio(switch_case, np2, mother_exten);
						switch_case-> return_target = np2;
					}
				} else if (p2->type == PV_DEFAULT) {
					/* ok, generate a extension and link it in */
					switch_case = new_exten();
					if (mother_exten && mother_exten->checked_switch) {
						switch_case->has_switch = mother_exten->has_switch;
						switch_case->checked_switch = mother_exten->checked_switch;
					}
					if (exten && exten->checked_switch) {
						switch_case->has_switch = exten->has_switch;
						switch_case->checked_switch = exten->checked_switch;
					}
					switch_case->context = this_context;
					switch_case->is_switch = 1;
					/* new: the default case intros a pattern with ., which covers ALMOST everything.
					   but it doesn't cover a NULL pattern. So, we'll define a null extension to match
					   that goto's the default extension. */
					default_exists++;
					switch_null = new_exten();
					if (mother_exten && mother_exten->checked_switch) {
						switch_null->has_switch = mother_exten->has_switch;
						switch_null->checked_switch = mother_exten->checked_switch;
					}
					if (exten && exten->checked_switch) {
						switch_null->has_switch = exten->has_switch;
						switch_null->checked_switch = exten->checked_switch;
					}
					switch_null->context = this_context;
					switch_null->is_switch = 1;
					switch_empty = new_prio();
					snprintf(buf1, BUF_SIZE, "sw_%d_.,10", local_control_statement_count);
					switch_empty->app = strdup("Goto");
					switch_empty->appargs = strdup(buf1);
					linkprio(switch_null, switch_empty, mother_exten);
					snprintf(buf1, BUF_SIZE, "sw_%d_", local_control_statement_count);
					switch_null->name = strdup(buf1);
					switch_null->loop_break = exten->loop_break;
					switch_null->loop_continue = exten->loop_continue;
					linkexten(exten,switch_null);
					/* the break/continue locations are inherited from parent */
					switch_case->loop_break = exten->loop_break;
					switch_case->loop_continue = exten->loop_continue;
					linkexten(exten,switch_case);
					snprintf(buf1, BUF_SIZE, "_sw_%d_.", local_control_statement_count);
					switch_case->name = strdup(buf1);
					snprintf(new_label, BUF_SIZE, "sw_%s_default_%d", label, local_control_statement_count);
					if (gen_prios(switch_case, new_label, p2->u2.statements, exten, this_context)) { /* this will link in all the default:  body statements here */
						return -1;
					}
					/* here is where we write code to "fall thru" to the next case... if there is one... */
					for (p3=p2->u2.statements; p3; p3=p3->next) {
						if (!p3->next)
							break;
					}
					/* p3 now points the last statement... */
					if (!p3 || (p3->type != PV_GOTO && p3->type != PV_BREAK && p3->type != PV_RETURN)) {
						/* is there a following CASE/PATTERN/DEFAULT? */
						if (p2->next && p2->next->type == PV_CASE) {
							fall_thru = new_prio();
							fall_thru->type = AEL_APPCALL;
							fall_thru->app = strdup("Goto");
							snprintf(buf1, BUF_SIZE, "sw_%d_%s,10", local_control_statement_count, p2->next->u1.str);
							fall_thru->appargs = strdup(buf1);
							linkprio(switch_case, fall_thru, mother_exten);
						} else if (p2->next && p2->next->type == PV_PATTERN) {
							fall_thru = new_prio();
							fall_thru->type = AEL_APPCALL;
							fall_thru->app = strdup("Goto");
							gen_match_to_pattern(p2->next->u1.str, buf2);
							snprintf(buf1, BUF_SIZE, "sw_%d_%s,10", local_control_statement_count, buf2);
							fall_thru->appargs = strdup(buf1);
							linkprio(switch_case, fall_thru, mother_exten);
						} else if (p2->next && p2->next->type == PV_DEFAULT) {
							fall_thru = new_prio();
							fall_thru->type = AEL_APPCALL;
							fall_thru->app = strdup("Goto");
							snprintf(buf1, BUF_SIZE, "sw_%d_.,10", local_control_statement_count);
							fall_thru->appargs = strdup(buf1);
							linkprio(switch_case, fall_thru, mother_exten);
						} else if (!p2->next) {
							fall_thru = new_prio();
							fall_thru->type = AEL_CONTROL1;
							fall_thru->goto_true = switch_end;
							fall_thru->app = strdup("Goto");
							linkprio(switch_case, fall_thru, mother_exten);
						}
					}
					if (switch_case->return_needed) { /* returns don't generate a goto eoe (end of extension) any more, just a Return() app call) */
						char buf[2000];
						struct ael_priority *np2 = new_prio();
						np2->type = AEL_APPCALL;
						np2->app = strdup("NoOp");
						snprintf(buf,sizeof(buf),"End of Extension %s", switch_case->name);
						np2->appargs = strdup(buf);
						linkprio(switch_case, np2, mother_exten);
						switch_case-> return_target = np2;
					}
				} else {
					/* what could it be??? */
				}
			}
			exten->loop_break = loop_break_save;
			exten->loop_continue = loop_continue_save;
			switch_test->origin = p;
			switch_end->origin = p;
			break;
		case PV_MACRO_CALL:
			pr = new_prio();
			pr->type = AEL_APPCALL;
			snprintf(buf1, BUF_SIZE, "%s,~~s~~,1", p->u1.str);
			first = 1;
			for (p2 = p->u2.arglist; p2; p2 = p2->next) {
				if (first)
				{
					strcat(buf1,"(");
					first = 0;
				}
				else
					strcat(buf1,",");
				strcat(buf1,p2->u1.str);
			}
			if (!first)
				strcat(buf1,")");
			pr->app = strdup("Gosub");
			pr->appargs = strdup(buf1);
			pr->origin = p;
			linkprio(exten, pr, mother_exten);
			break;
		case PV_APPLICATION_CALL:
			pr = new_prio();
			pr->type = AEL_APPCALL;
			buf1[0] = 0;
			for (p2 = p->u2.arglist; p2; p2 = p2->next) {
				if (p2 != p->u2.arglist )
					strcat(buf1,",");
				strcat(buf1,p2->u1.str);
			}
			pr->app = strdup(p->u1.str);
			pr->appargs = strdup(buf1);
			pr->origin = p;
			linkprio(exten, pr, mother_exten);
			break;
		case PV_BREAK:
			pr = new_prio();
			pr->type = AEL_CONTROL1; /* simple goto */
			pr->goto_true = exten->loop_break;
			pr->origin = p;
			linkprio(exten, pr, mother_exten);
			break;
		case PV_RETURN: /* hmmmm */
			pr = new_prio();
			pr->type = AEL_RETURN; /* simple Return */
			/* exten->return_needed++; */
			pr->app = strdup("Return");
			pr->appargs = strdup("");
			pr->origin = p;
			linkprio(exten, pr, mother_exten);
			break;
		case PV_CONTINUE:
			pr = new_prio();
			pr->type = AEL_CONTROL1; /* simple goto */
			pr->goto_true = exten->loop_continue;
			pr->origin = p;
			linkprio(exten, pr, mother_exten);
			break;
		case PV_IFTIME:
			control_statement_count++;
			snprintf(new_label, BUF_SIZE, "iftime_%s_%d", label, control_statement_count);
			if_test = new_prio();
			if_test->type = AEL_IFTIME_CONTROL;
			snprintf(buf1, BUF_SIZE, "%s,%s,%s,%s",
					 p->u1.list->u1.str, 
					 p->u1.list->next->u1.str, 
					 p->u1.list->next->next->u1.str, 
					 p->u1.list->next->next->next->u1.str);
			if_test->app = 0;
			if_test->appargs = strdup(buf1);
			if_test->origin = p;
			if_end = new_prio();
			if_end->type = AEL_APPCALL;
			snprintf(buf1, BUF_SIZE, "Finish iftime_%s_%d", label, control_statement_count);
			if_end->app = strdup("NoOp");
			if_end->appargs = strdup(buf1);
			if (p->u3.else_statements) {
				if_skip = new_prio();
				if_skip->type = AEL_CONTROL1; /* simple goto */
				if_skip->goto_true = if_end;
				if_skip->origin  = p;
			} else {
				if_skip = 0;
				if_test->goto_false = if_end;
			}
			if_false = new_prio();
			if_false->type = AEL_CONTROL1;
			if (p->u3.else_statements) {
				if_false->goto_true = if_skip; /* +1 */
			} else {
				if_false->goto_true = if_end;
			}
			/* link & load! */
			linkprio(exten, if_test, mother_exten);
			linkprio(exten, if_false, mother_exten);
			/* now, put the body of the if here */
			if (gen_prios(exten, new_label, p->u2.statements, mother_exten, this_context)) { /* this will link in all the statements here */
				return -1;
			}
			if (p->u3.else_statements) {
				linkprio(exten, if_skip, mother_exten);
				if (gen_prios(exten, new_label, p->u3.else_statements, mother_exten, this_context)) { /* this will link in all the statements here */
					return -1;
				}
			}
			linkprio(exten, if_end, mother_exten);
			break;
		case PV_RANDOM:
		case PV_IF:
			control_statement_count++;
			snprintf(new_label, BUF_SIZE, "if_%s_%d", label, control_statement_count);
			if_test = new_prio();
			if_end = new_prio();
			if_test->type = AEL_IF_CONTROL;
			if_end->type = AEL_APPCALL;
			if ( p->type == PV_RANDOM )
				snprintf(buf1, BUF_SIZE, "$[${RAND(0,99)} < (%s)]", p->u1.str);
			else
				snprintf(buf1, BUF_SIZE, "$[%s]", p->u1.str);
			if_test->app = 0;
			if_test->appargs = strdup(buf1);
			snprintf(buf1, BUF_SIZE, "Finish if_%s_%d", label, control_statement_count);
			if_end->app = strdup("NoOp");
			if_end->appargs = strdup(buf1);
			if_test->origin = p;
			if (p->u3.else_statements) {
				if_skip = new_prio();
				if_skip->type = AEL_CONTROL1; /* simple goto */
				if_skip->goto_true = if_end;
				if_test->goto_false = if_skip;;
			} else {
				if_skip = 0;
				if_test->goto_false = if_end;;
			}
			/* link & load! */
			linkprio(exten, if_test, mother_exten);
			/* now, put the body of the if here */
			if (gen_prios(exten, new_label, p->u2.statements, mother_exten, this_context)) { /* this will link in all the statements here */
				return -1;
			}
			if (p->u3.else_statements) {
				linkprio(exten, if_skip, mother_exten);
				if (gen_prios(exten, new_label, p->u3.else_statements, mother_exten, this_context)) { /* this will link in all the statements here */
					return -1;
				}
			}
			linkprio(exten, if_end, mother_exten);
			break;
		case PV_STATEMENTBLOCK:
			if (gen_prios(exten, label, p->u1.list, mother_exten, this_context)) { /* recurse into the block */
				return -1;
			}
			break;
		case PV_CATCH:
			control_statement_count++;
			/* generate an extension with name of catch, put all catch stats
			   into this exten! */
			switch_case = new_exten();
			if (mother_exten && mother_exten->checked_switch) {
				switch_case->has_switch = mother_exten->has_switch;
				switch_case->checked_switch = mother_exten->checked_switch;
			}
			if (exten && exten->checked_switch) {
				switch_case->has_switch = exten->has_switch;
				switch_case->checked_switch = exten->checked_switch;
			}
			switch_case->context = this_context;
			linkexten(exten,switch_case);
			switch_case->name = strdup(p->u1.str);
			snprintf(new_label, BUF_SIZE, "catch_%s_%d",p->u1.str, control_statement_count);
			if (gen_prios(switch_case, new_label, p->u2.statements, mother_exten,this_context)) { /* this will link in all the catch body statements here */
				return -1;
			}
			if (switch_case->return_needed) { /* returns now generate a Return() app call, no longer a goto to the end of the exten */
				char buf[2000];
				struct ael_priority *np2 = new_prio();
				np2->type = AEL_APPCALL;
				np2->app = strdup("NoOp");
				snprintf(buf,sizeof(buf),"End of Extension %s", switch_case->name);
				np2->appargs = strdup(buf);
				linkprio(switch_case, np2, mother_exten);
				switch_case-> return_target = np2;
			}
			break;
		default:
			break;
		}
	}
	return 0;
}
