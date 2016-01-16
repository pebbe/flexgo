/* gen - actual generation (writing) of flex scanners */

/*  Copyright (c) 1990 The Regents of the University of California. */
/*  All rights reserved. */

/*  This code is derived from software contributed to Berkeley by */
/*  Vern Paxson. */

/*  The United States Government has rights in this work pursuant */
/*  to contract no. DE-AC03-76SF00098 between the United States */
/*  Department of Energy and the University of California. */

/*  This file is part of flex. */

/*  Redistribution and use in source and binary forms, with or without */
/*  modification, are permitted provided that the following conditions */
/*  are met: */

/*  1. Redistributions of source code must retain the above copyright */
/*     notice, this list of conditions and the following disclaimer. */
/*  2. Redistributions in binary form must reproduce the above copyright */
/*     notice, this list of conditions and the following disclaimer in the */
/*     documentation and/or other materials provided with the distribution. */

/*  Neither the name of the University nor the names of its contributors */
/*  may be used to endorse or promote products derived from this software */
/*  without specific prior written permission. */

/*  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR */
/*  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR */
/*  PURPOSE. */

#include "flexdef.h"
#include "tables.h"


/* declare functions that have forward references */

void gen_next_state_go PROTO ((int));
void genecs_go PROTO ((void));
void indent_put2s_go PROTO ((const char *, const char *));
void indent_puts_go PROTO ((const char *));


static int indent_level_go = 0;

#define indent_up_go() (++indent_level_go)
#define indent_down_go() (--indent_level_go)
#define set_indent_go(indent_val_go) indent_level_go = indent_val_go

/* Almost everything is done in terms of arrays starting at 1, so provide
 * a null entry for the zero element of all C arrays.  (The exception
 * to this is that the fast table representation generally uses the
 * 0 elements of its arrays, too.)
 */

static const char *get_int16_decl_go (void)
{
    return (gentables)
	? "var %s = [%d]int16{   0,\n"
	: "var %s []int16\n";
}


static const char *get_int32_decl_go (void)
{
    return (gentables)
	? "var %s = [%d]int32{   0,\n"
	: "var %s []int32\n";
}

static const char *get_state_decl_go (void)
{
    return (gentables)
	? "var %s = [%d]int{   0,\n"
	: "var %s []int\n";
}

static const char *get_uint16_decl_go (void)
{
    return (gentables)
	? "var %s = [%d]uint16{   0,\n"
	: "var %s []uint16\n";
}

static const char *get_uint32_decl_go (void)
{
    return (gentables)
	? "var %s = [%d]uint32{     0,\n"
	: "var %s []uint32\n";
}

static const char *get_yy_char_decl_go (void)
{
    return (gentables)
	? "var %s = [%d]byte{    0,\n"
	: "var %s []byte\n";
}

/* Indent to the current level. */

void do_indent_go (void)
{
    int i = indent_level_go * 8;

    while (i >= 8) {
	outc ('\t');
	i -= 8;
    }

    while (i > 0) {
	outc (' ');
	--i;
    }
}


/** Make the table for possible eol matches.
 *  @return the newly allocated rule_can_match_eol table
 */
static struct yytbl_data *mkeoltbl_go (void)
{
    int     i;
    flex_int8_t *tdata = 0;
    struct yytbl_data *tbl;

    tbl = (struct yytbl_data *) calloc (1, sizeof (struct yytbl_data));
    yytbl_data_init (tbl, YYTD_ID_RULE_CAN_MATCH_EOL);
    tbl->td_flags = YYTD_DATA8;
    tbl->td_lolen = num_rules + 1;
    tbl->td_data = tdata =
	(flex_int8_t *) calloc (tbl->td_lolen, sizeof (flex_int8_t));

    for (i = 1; i <= num_rules; i++)
	tdata[i] = rule_has_nl[i] ? 1 : 0;

    buf_prints (&yydmap_buf,
		"\t{YYTD_ID_RULE_CAN_MATCH_EOL, (void**)&yy_rule_can_match_eol, sizeof(%s)},\n",
		"flex_int32_t");
    return tbl;
}

/* Generate the table for possible eol matches. */
static void geneoltbl_go (void)
{
    int     i;

    outn ("m4_ifdef( [[M4_YY_USE_LINENO]],[[");
    outn ("/* Table of booleans, true if rule could match eol. */");
    out_str_dec (get_int32_decl_go (), "yy_rule_can_match_eol",
		 num_rules + 1);

    if (gentables) {
	for (i = 1; i <= num_rules; i++) {
	    out_dec ("%d, ", rule_has_nl[i] ? 1 : 0);
	    /* format nicely, 20 numbers per line. */
	    if ((i % 20) == 19)
		out ("\n    ");
	}
	out ("    };\n");
    }
    outn ("]])");
}


/* Generate the code to keep backing-up information. */

void gen_backing_up_go ()
{
    if (reject || num_backing_up == 0)
	return;

    if (fullspd)
	indent_puts_go ("if yy_current_state[-1].yy_nxt {");
    else
	indent_puts_go ("if yy_accept[yy_current_state] != 0 {");

    indent_up_go ();
    indent_puts_go ("yy_last_accepting_state = yy_current_state");
    indent_puts_go ("yy_last_accepting_cpos = yy_cp");
    indent_down_go ();
    indent_puts_go ("}");
}


/* Generate the code to perform the backing up. */

void gen_bu_action_go (void)
{
    if (reject || num_backing_up == 0)
	return;

    set_indent_go (3);

    indent_puts_go ("case 0: /* must back up */");
    indent_puts_go ("buffer.clearNul()");
    indent_puts_go ("yy_cp = yy_last_accepting_cpos");
    indent_puts_go ("yy_current_state = yy_last_accepting_state");
    indent_puts_go ("goto yy_find_action");

    /*
    if (fullspd || fulltbl)
	indent_puts_go ("yy_cp = YY_G(yy_last_accepting_cpos) + 1;");
    else
        // Backing-up info for compressed tables is taken \after/
	// yy_cp has been incremented for the next state.
	indent_puts_go ("yy_cp = YY_G(yy_last_accepting_cpos);");

    indent_puts_go ("yy_current_state = YY_G(yy_last_accepting_state);");
    indent_puts_go ("goto yy_find_action;");
    outc ('\n');

    */

    set_indent_go (0);
}

/** mkctbl - make full speed compressed transition table
 * This is an array of structs; each struct a pair of integers.
 * You should call mkssltbl() immediately after this.
 * Then, I think, mkecstbl(). Arrrg.
 * @return the newly allocated trans table
 */

static struct yytbl_data *mkctbl_go (void)
{
    int i;
    struct yytbl_data *tbl = 0;
    flex_int32_t *tdata = 0, curr = 0;
    int     end_of_buffer_action = num_rules + 1;

    buf_prints (&yydmap_buf,
		"\t{YYTD_ID_TRANSITION, (void**)&yy_transition, sizeof(%s)},\n",
		((tblend + numecs + 1) >= INT16_MAX
		 || long_align) ? "flex_int32_t" : "flex_int16_t");

    tbl = (struct yytbl_data *) calloc (1, sizeof (struct yytbl_data));
    yytbl_data_init (tbl, YYTD_ID_TRANSITION);
    tbl->td_flags = YYTD_DATA32 | YYTD_STRUCT;
    tbl->td_hilen = 0;
    tbl->td_lolen = tblend + numecs + 1;	/* number of structs */

    tbl->td_data = tdata =
	(flex_int32_t *) calloc (tbl->td_lolen * 2, sizeof (flex_int32_t));

    /* We want the transition to be represented as the offset to the
     * next state, not the actual state number, which is what it currently
     * is.  The offset is base[nxt[i]] - (base of current state)].  That's
     * just the difference between the starting points of the two involved
     * states (to - from).
     *
     * First, though, we need to find some way to put in our end-of-buffer
     * flags and states.  We do this by making a state with absolutely no
     * transitions.  We put it at the end of the table.
     */

    /* We need to have room in nxt/chk for two more slots: One for the
     * action and one for the end-of-buffer transition.  We now *assume*
     * that we're guaranteed the only character we'll try to index this
     * nxt/chk pair with is EOB, i.e., 0, so we don't have to make sure
     * there's room for jam entries for other characters.
     */

    while (tblend + 2 >= current_max_xpairs)
	expand_nxt_chk ();

    while (lastdfa + 1 >= current_max_dfas)
	increase_max_dfas ();

    base[lastdfa + 1] = tblend + 2;
    nxt[tblend + 1] = end_of_buffer_action;
    chk[tblend + 1] = numecs + 1;
    chk[tblend + 2] = 1;	/* anything but EOB */

    /* So that "make test" won't show arb. differences. */
    nxt[tblend + 2] = 0;

    /* Make sure every state has an end-of-buffer transition and an
     * action #.
     */
    for (i = 0; i <= lastdfa; ++i) {
	int     anum = dfaacc[i].dfaacc_state;
	int     offset = base[i];

	chk[offset] = EOB_POSITION;
	chk[offset - 1] = ACTION_POSITION;
	nxt[offset - 1] = anum;	/* action number */
    }

    for (i = 0; i <= tblend; ++i) {
	if (chk[i] == EOB_POSITION) {
	    tdata[curr++] = 0;
	    tdata[curr++] = base[lastdfa + 1] - i;
	}

	else if (chk[i] == ACTION_POSITION) {
	    tdata[curr++] = 0;
	    tdata[curr++] = nxt[i];
	}

	else if (chk[i] > numecs || chk[i] == 0) {
	    tdata[curr++] = 0;
	    tdata[curr++] = 0;
	}
	else {		/* verify, transition */

	    tdata[curr++] = chk[i];
	    tdata[curr++] = base[nxt[i]] - (i - chk[i]);
	}
    }


    /* Here's the final, end-of-buffer state. */
    tdata[curr++] = chk[tblend + 1];
    tdata[curr++] = nxt[tblend + 1];

    tdata[curr++] = chk[tblend + 2];
    tdata[curr++] = nxt[tblend + 2];

    return tbl;
}


/** Make start_state_list table.
 *  @return the newly allocated start_state_list table
 */
static struct yytbl_data *mkssltbl_go (void)
{
    struct yytbl_data *tbl = 0;
    flex_int32_t *tdata = 0;
    flex_int32_t i;

    tbl = (struct yytbl_data *) calloc (1, sizeof (struct yytbl_data));
    yytbl_data_init (tbl, YYTD_ID_START_STATE_LIST);
    tbl->td_flags = YYTD_DATA32 | YYTD_PTRANS;
    tbl->td_hilen = 0;
    tbl->td_lolen = lastsc * 2 + 1;

    tbl->td_data = tdata =
	(flex_int32_t *) calloc (tbl->td_lolen, sizeof (flex_int32_t));

    for (i = 0; i <= lastsc * 2; ++i)
	tdata[i] = base[i];

    buf_prints (&yydmap_buf,
		"\t{YYTD_ID_START_STATE_LIST, (void**)&yy_start_state_list, sizeof(%s)},\n",
		"struct yy_trans_info*");

    return tbl;
}



/* genctbl - generates full speed compressed transition table */

void genctbl_go (void)
{
    int i;
    int     end_of_buffer_action = num_rules + 1;

    /* Table of verify for transition and offset to next state. */
    if (gentables)
	out_dec ("static yyconst struct yy_trans_info yy_transition[%d] =\n    {\n", tblend + numecs + 1);
    else
	outn ("static yyconst struct yy_trans_info *yy_transition = 0;");

    /* We want the transition to be represented as the offset to the
     * next state, not the actual state number, which is what it currently
     * is.  The offset is base[nxt[i]] - (base of current state)].  That's
     * just the difference between the starting points of the two involved
     * states (to - from).
     *
     * First, though, we need to find some way to put in our end-of-buffer
     * flags and states.  We do this by making a state with absolutely no
     * transitions.  We put it at the end of the table.
     */

    /* We need to have room in nxt/chk for two more slots: One for the
     * action and one for the end-of-buffer transition.  We now *assume*
     * that we're guaranteed the only character we'll try to index this
     * nxt/chk pair with is EOB, i.e., 0, so we don't have to make sure
     * there's room for jam entries for other characters.
     */

    while (tblend + 2 >= current_max_xpairs)
	expand_nxt_chk ();

    while (lastdfa + 1 >= current_max_dfas)
	increase_max_dfas ();

    base[lastdfa + 1] = tblend + 2;
    nxt[tblend + 1] = end_of_buffer_action;
    chk[tblend + 1] = numecs + 1;
    chk[tblend + 2] = 1;	/* anything but EOB */

    /* So that "make test" won't show arb. differences. */
    nxt[tblend + 2] = 0;

    /* Make sure every state has an end-of-buffer transition and an
     * action #.
     */
    for (i = 0; i <= lastdfa; ++i) {
	int     anum = dfaacc[i].dfaacc_state;
	int     offset = base[i];

	chk[offset] = EOB_POSITION;
	chk[offset - 1] = ACTION_POSITION;
	nxt[offset - 1] = anum;	/* action number */
    }

    for (i = 0; i <= tblend; ++i) {
	if (chk[i] == EOB_POSITION)
	    transition_struct_out (0, base[lastdfa + 1] - i);

	else if (chk[i] == ACTION_POSITION)
	    transition_struct_out (0, nxt[i]);

	else if (chk[i] > numecs || chk[i] == 0)
	    transition_struct_out (0, 0);	/* unused slot */

	else		/* verify, transition */
	    transition_struct_out (chk[i],
				   base[nxt[i]] - (i -
						   chk[i]));
    }


    /* Here's the final, end-of-buffer state. */
    transition_struct_out (chk[tblend + 1], nxt[tblend + 1]);
    transition_struct_out (chk[tblend + 2], nxt[tblend + 2]);

    if (gentables)
	outn ("    };\n");

    /* Table of pointers to start states. */
    if (gentables)
	out_dec ("static yyconst struct yy_trans_info *yy_start_state_list[%d] =\n", lastsc * 2 + 1);
    else
	outn ("static yyconst struct yy_trans_info **yy_start_state_list =0;");

    if (gentables) {
	outn ("    {");

	for (i = 0; i <= lastsc * 2; ++i)
	    out_dec ("    &yy_transition[%d],\n", base[i]);

	dataend ();
    }

    if (useecs)
	genecs_go ();
}


/* mkecstbl - Make equivalence-class tables.  */

struct yytbl_data *mkecstbl_go (void)
{
    int i;
    struct yytbl_data *tbl = 0;
    flex_int32_t *tdata = 0;

    tbl = (struct yytbl_data *) calloc (1, sizeof (struct yytbl_data));
    yytbl_data_init (tbl, YYTD_ID_EC);
    tbl->td_flags |= YYTD_DATA32;
    tbl->td_hilen = 0;
    tbl->td_lolen = csize;

    tbl->td_data = tdata =
	(flex_int32_t *) calloc (tbl->td_lolen, sizeof (flex_int32_t));

    for (i = 1; i < csize; ++i) {
	ecgroup[i] = ABS (ecgroup[i]);
	tdata[i] = ecgroup[i];
    }

    buf_prints (&yydmap_buf,
		"\t{YYTD_ID_EC, (void**)&yy_ec, sizeof(%s)},\n",
		"YY_CHAR");

    return tbl;
}

/* Generate equivalence-class tables. */

void genecs_go (void)
{
    int i, j;
    int     numrows;

    out_str_dec (get_yy_char_decl_go (), "yy_ec", csize);

    for (i = 1; i < csize; ++i) {
	ecgroup[i] = ABS (ecgroup[i]);
	mkdata (ecgroup[i]);
    }

    dataend ();

    if (trace) {
	fputs (_("\n\nEquivalence Classes:\n\n"), stderr);

	numrows = csize / 8;

	for (j = 0; j < numrows; ++j) {
	    for (i = j; i < csize; i = i + numrows) {
		fprintf (stderr, "%4s = %-2d",
			 readable_form (i), ecgroup[i]);

		putc (' ', stderr);
	    }

	    putc ('\n', stderr);
	}
    }
}


/* Generate the code to find the action number. */

void gen_find_action_go (void)
{
    if (fullspd)
	indent_puts_go ("yy_act = yy_current_state[-1].yy_nxt");

    else if (fulltbl)
	indent_puts_go ("yy_act = int(yy_accept[yy_current_state])");

    else if (reject) {
	indent_puts_go ("yy_current_state = *--YY_G(yy_state_ptr);");
	indent_puts_go ("YY_G(yy_lp) = yy_accept[yy_current_state];");

	if(reject_really_used)
	    outn ("find_rule: /* we branch to this label when backing up */");

	indent_puts_go
	    ("for ( ; ; ) /* until we find what rule we matched */");

	indent_up_go ();

	indent_puts_go ("{");

	indent_puts_go
	    ("if ( YY_G(yy_lp) && YY_G(yy_lp) < yy_accept[yy_current_state + 1] )");
	indent_up_go ();
	indent_puts_go ("{");
	indent_puts_go ("yy_act = yy_acclist[YY_G(yy_lp)];");

	if (variable_trailing_context_rules) {
	    indent_puts_go
		("if ( yy_act & YY_TRAILING_HEAD_MASK ||");
	    indent_puts_go ("     YY_G(yy_looking_for_trail_begin) )");
	    indent_up_go ();
	    indent_puts_go ("{");

	    indent_puts_go
		("if ( yy_act == YY_G(yy_looking_for_trail_begin) )");
	    indent_up_go ();
	    indent_puts_go ("{");
	    indent_puts_go ("YY_G(yy_looking_for_trail_begin) = 0;");
	    indent_puts_go ("yy_act &= ~YY_TRAILING_HEAD_MASK;");
	    indent_puts_go ("break;");
	    indent_puts_go ("}");
	    indent_down_go ();

	    indent_puts_go ("}");
	    indent_down_go ();

	    indent_puts_go
		("else if ( yy_act & YY_TRAILING_MASK )");
	    indent_up_go ();
	    indent_puts_go ("{");
	    indent_puts_go
		("YY_G(yy_looking_for_trail_begin) = yy_act & ~YY_TRAILING_MASK;");
	    indent_puts_go
		("YY_G(yy_looking_for_trail_begin) |= YY_TRAILING_HEAD_MASK;");

	    if (real_reject) {
		/* Remember matched text in case we back up
		 * due to REJECT.
		 */
		indent_puts_go
		    ("YY_G(yy_full_match) = yy_cp;");
		indent_puts_go
		    ("YY_G(yy_full_state) = YY_G(yy_state_ptr);");
		indent_puts_go ("YY_G(yy_full_lp) = YY_G(yy_lp);");
	    }

	    indent_puts_go ("}");
	    indent_down_go ();

	    indent_puts_go ("else");
	    indent_up_go ();
	    indent_puts_go ("{");
	    indent_puts_go ("YY_G(yy_full_match) = yy_cp;");
	    indent_puts_go
		("YY_G(yy_full_state) = YY_G(yy_state_ptr);");
	    indent_puts_go ("YY_G(yy_full_lp) = YY_G(yy_lp);");
	    indent_puts_go ("break;");
	    indent_puts_go ("}");
	    indent_down_go ();

	    indent_puts_go ("++YY_G(yy_lp);");
	    indent_puts_go ("goto find_rule;");
	}

	else {
	    /* Remember matched text in case we back up due to
	     * trailing context plus REJECT.
	     */
	    indent_up_go ();
	    indent_puts_go ("{");
	    indent_puts_go ("YY_G(yy_full_match) = yy_cp;");
	    indent_puts_go ("break;");
	    indent_puts_go ("}");
	    indent_down_go ();
	}

	indent_puts_go ("}");
	indent_down_go ();

	indent_puts_go ("--yy_cp;");

	/* We could consolidate the following two lines with those at
	 * the beginning, but at the cost of complaints that we're
	 * branching inside a loop.
	 */
	indent_puts_go ("yy_current_state = *--YY_G(yy_state_ptr);");
	indent_puts_go ("YY_G(yy_lp) = yy_accept[yy_current_state];");

	indent_puts_go ("}");

	indent_down_go ();
    }

    else {			/* compressed */
	indent_puts_go ("yy_act = int(yy_accept[yy_current_state])");

	if (interactive && !reject) {
	    /* Do the guaranteed-needed backing up to figure out
	     * the match.
	     */
	    indent_puts_go ("if yy_act == 0 {");
	    indent_up_go ();
	    indent_puts_go ("/* have to back up */");
	    indent_puts_go ("buffer.Push(YYtext)");
	    indent_puts_go ("if yy_last_accepting_cpos > 0 {");
	    indent_up_go ();
	    indent_puts_go ("YYtext = buffer.Read(yy_last_accepting_cpos)");
	    indent_down_go ();
	    indent_puts_go ("} else {");
	    indent_up_go ();
	    indent_puts_go ("YYtext = []byte{}");
	    indent_down_go ();
	    indent_puts_go ("}");
	    indent_puts_go ("yy_current_state = yy_last_accepting_state");
	    indent_down_go ();
	    indent_puts_go ("}");

	}
    }
}

/* mkftbl - make the full table and return the struct .
 * you should call mkecstbl() after this.
 */

struct yytbl_data *mkftbl_go (void)
{
    int i;
    int     end_of_buffer_action = num_rules + 1;
    struct yytbl_data *tbl;
    flex_int32_t *tdata = 0;

    tbl = (struct yytbl_data *) calloc (1, sizeof (struct yytbl_data));
    yytbl_data_init (tbl, YYTD_ID_ACCEPT);
    tbl->td_flags |= YYTD_DATA32;
    tbl->td_hilen = 0;	/* it's a one-dimensional array */
    tbl->td_lolen = lastdfa + 1;

    tbl->td_data = tdata =
	(flex_int32_t *) calloc (tbl->td_lolen, sizeof (flex_int32_t));

    dfaacc[end_of_buffer_state].dfaacc_state = end_of_buffer_action;

    for (i = 1; i <= lastdfa; ++i) {
	int anum = dfaacc[i].dfaacc_state;

	tdata[i] = anum;

	if (trace && anum)
	    fprintf (stderr, _("state # %d accepts: [%d]\n"),
		     i, anum);
    }

    buf_prints (&yydmap_buf,
		"\t{YYTD_ID_ACCEPT, (void**)&yy_accept, sizeof(%s)},\n",
		long_align ? "flex_int32_t" : "flex_int16_t");
    return tbl;
}


/* genftbl - generate full transition table */

void genftbl_go (void)
{
    int i;
    int     end_of_buffer_action = num_rules + 1;

    out_str_dec (long_align ? get_int32_decl_go () : get_int16_decl_go (),
		 "yy_accept", lastdfa + 1);

    dfaacc[end_of_buffer_state].dfaacc_state = end_of_buffer_action;

    for (i = 1; i <= lastdfa; ++i) {
	int anum = dfaacc[i].dfaacc_state;

	mkdata (anum);

	if (trace && anum)
	    fprintf (stderr, _("state # %d accepts: [%d]\n"),
		     i, anum);
    }

    dataend ();

    if (useecs)
	genecs_go ();

    /* Don't have to dump the actual full table entries - they were
     * created on-the-fly.
     */
}


/* Generate the code to find the next compressed-table state. */

void gen_next_compressed_state_go (char *char_map)
{
    //indent_put2s_go ("YY_CHAR yy_c = %s;", char_map);

    indent_puts_go ("yy_cp := yy_c_buf_p");
    indent_puts_go ("yy_c := 1");

    /* Save the backing-up info \before/ computing the next state
     * because we always compute one more state than needed - we
     * always proceed until we reach a jam state
     */
    gen_backing_up_go ();

    indent_puts_go
	("for int(yy_chk[int(yy_base[yy_current_state])+yy_c]) != yy_current_state {");
    indent_up_go ();
    indent_puts_go ("yy_current_state = int(yy_def[yy_current_state])");

    if (usemecs) {
	/* We've arrange it so that templates are never chained
	 * to one another.  This means we can afford to make a
	 * very simple test to see if we need to convert to
	 * yy_c's meta-equivalence class without worrying
	 * about erroneously looking up the meta-equivalence
	 * class twice
	 */
	do_indent_go ();

	/* lastdfa + 2 is the beginning of the templates */
	out_dec ("if yy_current_state >= %d {\n", lastdfa + 2);

	indent_up_go ();
	indent_puts_go ("yy_c = int(yy_meta[yy_c])");
	indent_down_go ();
	indent_puts_go ("}");
    }

    indent_down_go ();
    indent_puts_go ("}");

    indent_puts_go
	("yy_current_state = int(yy_nxt[int(yy_base[yy_current_state])+yy_c])");
}


/* Generate the code to find the next match. */

void gen_next_match_go (void)
{
    /* NOTE - changes in here should be reflected in gen_next_state_go() and
     * gen_NUL_trans().
     */
    char   *char_map = useecs ?
	"int(yy_ec[curbyte])" : "int(curbyte)";

    char   *char_map_2 = useecs ?
	"yy_ec[YY_SC_TO_UI(*++yy_cp)] " : "YY_SC_TO_UI(*++yy_cp)";

    if (fulltbl) {
	if (gentables)
	    indent_put2s_go
		("while ( (yy_current_state = yy_nxt[yy_current_state][ %s ]) > 0 )",
		 char_map);
	else
	    indent_put2s_go
		("while ( (yy_current_state = yy_nxt[yy_current_state*YY_NXT_LOLEN +  %s ]) > 0 )",
		 char_map);

	indent_up_go ();

	if (num_backing_up > 0) {
	    indent_puts_go ("{");
	    gen_backing_up_go ();
	    outc ('\n');
	}

	indent_puts_go ("++yy_cp;");

	if (num_backing_up > 0)

	    indent_puts_go ("}");

	indent_down_go ();

	outc ('\n');
	indent_puts_go ("yy_current_state = -yy_current_state;");
    }

    else if (fullspd) {
	indent_puts_go ("{");
	indent_puts_go
	    ("yyconst struct yy_trans_info *yy_trans_info;\n");
	indent_puts_go ("YY_CHAR yy_c;\n");
	indent_put2s_go ("for ( yy_c = %s;", char_map);
	indent_puts_go
	    ("      (yy_trans_info = &yy_current_state[(unsigned int) yy_c])->");
	indent_puts_go ("yy_verify == yy_c;");
	indent_put2s_go ("      yy_c = %s )", char_map_2);

	indent_up_go ();

	if (num_backing_up > 0)
	    indent_puts_go ("{");

	indent_puts_go ("yy_current_state += yy_trans_info->yy_nxt;");

	if (num_backing_up > 0) {
	    outc ('\n');
	    gen_backing_up_go ();
	    indent_puts_go ("}");
	}

	indent_down_go ();
	indent_puts_go ("}");
    }

    else {			/* compressed */

	indent_puts_go( "for {");

	indent_up_go ();

	gen_next_state_go (false);

	indent_puts_go( "yy_cp++");

	do_indent_go ();

	if (interactive)
	    out_dec ("if yy_current_state == %d {\n", jambase);
	else
	    out_dec ("if yy_current_state == %d {\n",
		     jamstate);
	indent_up_go ();
	indent_puts_go ("break");
	indent_down_go ();
	indent_puts_go ("}");
	indent_down_go ();
	indent_puts_go ("}");

	if (!reject && !interactive) {
	    /* Do the guaranteed-needed backing up to figure out
	     * the match.
	     */
		    
	    indent_puts_go ("yy_cp = yy_last_accepting_cpos");
	    indent_puts_go ("yy_current_state = yy_last_accepting_state");

	}

    }
}


/* Generate the code to find the next state. */

void gen_next_state_go (int worry_about_NULs)
{				/* NOTE - changes in here should be reflected in gen_next_match_go() */
    char    char_map[256];

    if (worry_about_NULs && !nultrans) {
	if (useecs)
	    snprintf (char_map, sizeof(char_map),
		      "(*yy_cp ? yy_ec[YY_SC_TO_UI(*yy_cp)] : %d)",
		      NUL_ec);
	else
            snprintf (char_map, sizeof(char_map),
		      "(*yy_cp ? YY_SC_TO_UI(*yy_cp) : %d)",
		      NUL_ec);
    }

    else
	strcpy (char_map, useecs ?
		"yy_ec[YY_SC_TO_UI(*yy_cp)] " :
		"YY_SC_TO_UI(*yy_cp)");

    if (worry_about_NULs && nultrans) {
	if (!fulltbl && !fullspd)
	    /* Compressed tables back up *before* they match. */
	    gen_backing_up_go ();

	indent_puts_go ("if ( *yy_cp )");
	indent_up_go ();
	indent_puts_go ("{");
    }

    if (fulltbl) {
	if (gentables)
	    indent_put2s_go
		("yy_current_state = yy_nxt[yy_current_state][%s];",
		 char_map);
	else
	    indent_put2s_go
		("yy_current_state = yy_nxt[yy_current_state*YY_NXT_LOLEN + %s];",
		 char_map);
    }

    else if (fullspd)
	indent_put2s_go
	    ("yy_current_state += yy_current_state[%s].yy_nxt;",
	     char_map);

    else
	gen_next_compressed_state_go (char_map);

    if (worry_about_NULs && nultrans) {

	indent_puts_go ("}");
	indent_down_go ();
	indent_puts_go ("else");
	indent_up_go ();
	indent_puts_go
	    ("yy_current_state = yy_NUL_trans[yy_current_state];");
	indent_down_go ();
    }

    if (fullspd || fulltbl)
	gen_backing_up_go ();

    if (reject)
	indent_puts_go ("*YY_G(yy_state_ptr)++ = yy_current_state;");
}


/* Generate the code to make a NUL transition. */

void gen_NUL_trans_go (void)
{				/* NOTE - changes in here should be reflected in gen_next_match_go() */
    /* Only generate a definition for "yy_cp" if we'll generate code
     * that uses it.  Otherwise lint and the like complain.
     */
    int     need_backing_up = (num_backing_up > 0 && !reject);

    if (need_backing_up && (!nultrans || fullspd || fulltbl)) {
	/* We're going to need yy_cp lying around for the call
	 * below to gen_backing_up_go().
	 */
	/* indent_puts_go ("char *yy_cp = YY_G(yy_c_buf_p);"); */
    }

    outc ('\n');

    if (nultrans) {
	indent_puts_go
	    ("yy_current_state = yy_NUL_trans[yy_current_state];");
	indent_puts_go ("yy_is_jam = (yy_current_state == 0);");
    }

    else if (fulltbl) {
	do_indent_go ();
	if (gentables)
	    out_dec ("yy_current_state = yy_nxt[yy_current_state][%d];\n", NUL_ec);
	else
	    out_dec ("yy_current_state = yy_nxt[yy_current_state*YY_NXT_LOLEN + %d];\n", NUL_ec);
	indent_puts_go ("yy_is_jam = (yy_current_state <= 0);");
    }

    else if (fullspd) {
	do_indent_go ();
	out_dec ("int yy_c = %d;\n", NUL_ec);

	indent_puts_go
	    ("yyconst struct yy_trans_info *yy_trans_info;\n");
	indent_puts_go
	    ("yy_trans_info = &yy_current_state[(unsigned int) yy_c];");
	indent_puts_go ("yy_current_state += yy_trans_info->yy_nxt;");

	indent_puts_go
	    ("yy_is_jam = (yy_trans_info->yy_verify != yy_c);");
    }

    else {
	char    NUL_ec_str[20];

	snprintf (NUL_ec_str, sizeof(NUL_ec_str), "%d", NUL_ec);
	gen_next_compressed_state_go (NUL_ec_str);

	do_indent_go ();
	out_dec ("if yy_current_state == %d {\n", jamstate);
	indent_up_go ();
	indent_puts_go ("yy_is_jam = true");
	indent_down_go ();
	indent_puts_go ("}");

	if (reject) {
	    /* Only stack this state if it's a transition we
	     * actually make.  If we stack it on a jam, then
	     * the state stack and yy_c_buf_p get out of sync.
	     */
	    indent_puts_go ("if ( ! yy_is_jam )");
	    indent_up_go ();
	    indent_puts_go
		("*YY_G(yy_state_ptr)++ = yy_current_state;");
	    indent_down_go ();
	}
    }

    /* If we've entered an accepting state, back up; note that
     * compressed tables have *already* done such backing up, so
     * we needn't bother with it again.
     */
    if (need_backing_up && (fullspd || fulltbl)) {
	outc ('\n');
	indent_puts_go ("if ( ! yy_is_jam )");
	indent_up_go ();
	indent_puts_go ("{");
	gen_backing_up_go ();
	indent_puts_go ("}");
	indent_down_go ();
    }
}


/* Generate the code to find the start state. */

void gen_start_state_go (void)
{
    if (fullspd) {
	if (bol_needed) {
	    indent_puts_go
		("yy_current_state = yy_start_state_list[yy_start + YY_AT_BOL()]");
	}
	else
	    indent_puts_go
		("yy_current_state = yy_start_state_list[yy_start]");
    }

    else {
	indent_puts_go ("yy_current_state = yy_start");

	if (bol_needed)
	    indent_puts_go ("yy_current_state += YY_AT_BOL()");

	if (reject) {
	    /* Set up for storing up states. */
	    outn ("m4_ifdef( [[M4_YY_USES_REJECT]],\n[[");
	    indent_puts_go
		("yy_state_ptr = yy_state_buf");
	    indent_puts_go
		("// TODO *YY_G(yy_state_ptr)++ = yy_current_state;");
	    outn ("]])");
	}
    }
}


/* gentabs - generate data statements for the transition tables */

void gentabs_go (void)
{
    int     i, j, k, *accset, nacc, *acc_array, total_states;
    int     end_of_buffer_action = num_rules + 1;
    struct yytbl_data *yyacc_tbl = 0, *yymeta_tbl = 0, *yybase_tbl = 0,
	*yydef_tbl = 0, *yynxt_tbl = 0, *yychk_tbl = 0, *yyacclist_tbl=0;
    flex_int32_t *yyacc_data = 0, *yybase_data = 0, *yydef_data = 0,
	*yynxt_data = 0, *yychk_data = 0, *yyacclist_data=0;
    flex_int32_t yybase_curr = 0, yyacclist_curr=0,yyacc_curr=0;

    acc_array = allocate_integer_array (current_max_dfas);
    nummt = 0;

    /* The compressed table format jams by entering the "jam state",
     * losing information about the previous state in the process.
     * In order to recover the previous state, we effectively need
     * to keep backing-up information.
     */
    ++num_backing_up;

    if (reject) {
	/* Write out accepting list and pointer list.

	 * First we generate the "yy_acclist" array.  In the process,
	 * we compute the indices that will go into the "yy_accept"
	 * array, and save the indices in the dfaacc array.
	 */
	int     EOB_accepting_list[2];

	/* Set up accepting structures for the End Of Buffer state. */
	EOB_accepting_list[0] = 0;
	EOB_accepting_list[1] = end_of_buffer_action;
	accsiz[end_of_buffer_state] = 1;
	dfaacc[end_of_buffer_state].dfaacc_set =
	    EOB_accepting_list;

	out_str_dec (long_align ? get_int32_decl_go () :
		     get_int16_decl_go (), "yy_acclist", MAX (numas,
							      1) + 1);
        
        buf_prints (&yydmap_buf,
		    "\t{YYTD_ID_ACCLIST, (void**)&yy_acclist, sizeof(%s)},\n",
		    long_align ? "flex_int32_t" : "flex_int16_t");

        yyacclist_tbl = (struct yytbl_data*)calloc(1,sizeof(struct yytbl_data));
        yytbl_data_init (yyacclist_tbl, YYTD_ID_ACCLIST);
        yyacclist_tbl->td_lolen  = MAX(numas,1) + 1;
        yyacclist_tbl->td_data = yyacclist_data = 
            (flex_int32_t *) calloc (yyacclist_tbl->td_lolen, sizeof (flex_int32_t));
        yyacclist_curr = 1;

	j = 1;		/* index into "yy_acclist" array */

	for (i = 1; i <= lastdfa; ++i) {
	    acc_array[i] = j;

	    if (accsiz[i] != 0) {
		accset = dfaacc[i].dfaacc_set;
		nacc = accsiz[i];

		if (trace)
		    fprintf (stderr,
			     _("state # %d accepts: "),
			     i);

		for (k = 1; k <= nacc; ++k) {
		    int     accnum = accset[k];

		    ++j;

		    if (variable_trailing_context_rules
			&& !(accnum &
			     YY_TRAILING_HEAD_MASK)
			&& accnum > 0
			&& accnum <= num_rules
			&& rule_type[accnum] ==
			RULE_VARIABLE) {
			/* Special hack to flag
			 * accepting number as part
			 * of trailing context rule.
			 */
			accnum |= YY_TRAILING_MASK;
		    }

		    mkdata (accnum);
                    yyacclist_data[yyacclist_curr++] = accnum;

		    if (trace) {
			fprintf (stderr, "[%d]",
				 accset[k]);

			if (k < nacc)
			    fputs (", ",
				   stderr);
			else
			    putc ('\n',
				  stderr);
		    }
		}
	    }
	}

	/* add accepting number for the "jam" state */
	acc_array[i] = j;

	dataend ();
        if (tablesext) {
            yytbl_data_compress (yyacclist_tbl);
            if (yytbl_data_fwrite (&tableswr, yyacclist_tbl) < 0)
                flexerror (_("Could not write yyacclist_tbl"));
            yytbl_data_destroy (yyacclist_tbl);
            yyacclist_tbl = NULL;
        }
    }

    else {
	dfaacc[end_of_buffer_state].dfaacc_state =
	    end_of_buffer_action;

	for (i = 1; i <= lastdfa; ++i)
	    acc_array[i] = dfaacc[i].dfaacc_state;

	/* add accepting number for jam state */
	acc_array[i] = 0;
    }

    /* Begin generating yy_accept */

    /* Spit out "yy_accept" array.  If we're doing "reject", it'll be
     * pointers into the "yy_acclist" array.  Otherwise it's actual
     * accepting numbers.  In either case, we just dump the numbers.
     */

    /* "lastdfa + 2" is the size of "yy_accept"; includes room for C arrays
     * beginning at 0 and for "jam" state.
     */
    k = lastdfa + 2;

    if (reject)
	/* We put a "cap" on the table associating lists of accepting
	 * numbers with state numbers.  This is needed because we tell
	 * where the end of an accepting list is by looking at where
	 * the list for the next state starts.
	 */
	++k;

    out_str_dec (long_align ? get_int32_decl_go () : get_int16_decl_go (),
		 "yy_accept", k);

    buf_prints (&yydmap_buf,
		"\t{YYTD_ID_ACCEPT, (void**)&yy_accept, sizeof(%s)},\n",
		long_align ? "flex_int32_t" : "flex_int16_t");

    yyacc_tbl =
	(struct yytbl_data *) calloc (1,
				      sizeof (struct yytbl_data));
    yytbl_data_init (yyacc_tbl, YYTD_ID_ACCEPT);
    yyacc_tbl->td_lolen = k;
    yyacc_tbl->td_data = yyacc_data =
	(flex_int32_t *) calloc (yyacc_tbl->td_lolen, sizeof (flex_int32_t));
    yyacc_curr=1;

    for (i = 1; i <= lastdfa; ++i) {
	mkdata (acc_array[i]);
	yyacc_data[yyacc_curr++] = acc_array[i];

	if (!reject && trace && acc_array[i])
	    fprintf (stderr, _("state # %d accepts: [%d]\n"),
		     i, acc_array[i]);
    }

    /* Add entry for "jam" state. */
    mkdata (acc_array[i]);
    yyacc_data[yyacc_curr++] = acc_array[i];

    if (reject) {
	/* Add "cap" for the list. */
	mkdata (acc_array[i]);
	yyacc_data[yyacc_curr++] = acc_array[i];
    }

    dataend ();
    if (tablesext) {
	yytbl_data_compress (yyacc_tbl);
	if (yytbl_data_fwrite (&tableswr, yyacc_tbl) < 0)
	    flexerror (_("Could not write yyacc_tbl"));
	yytbl_data_destroy (yyacc_tbl);
	yyacc_tbl = NULL;
    }
    /* End generating yy_accept */

    if (useecs) {

	genecs_go ();
	if (tablesext) {
	    struct yytbl_data *tbl;

	    tbl = mkecstbl ();
	    yytbl_data_compress (tbl);
	    if (yytbl_data_fwrite (&tableswr, tbl) < 0)
		flexerror (_("Could not write ecstbl"));
	    yytbl_data_destroy (tbl);
	    tbl = 0;
	}
    }

    if (usemecs) {
	/* Begin generating yy_meta */
	/* Write out meta-equivalence classes (used to index
	 * templates with).
	 */
	flex_int32_t *yymecs_data = 0;
	yymeta_tbl =
	    (struct yytbl_data *) calloc (1,
					  sizeof (struct
						  yytbl_data));
	yytbl_data_init (yymeta_tbl, YYTD_ID_META);
	yymeta_tbl->td_lolen = numecs + 1;
	yymeta_tbl->td_data = yymecs_data =
	    (flex_int32_t *) calloc (yymeta_tbl->td_lolen,
				     sizeof (flex_int32_t));

	if (trace)
	    fputs (_("\n\nMeta-Equivalence Classes:\n"),
		   stderr);

	out_str_dec (get_yy_char_decl_go (), "yy_meta", numecs + 1);
	buf_prints (&yydmap_buf,
		    "\t{YYTD_ID_META, (void**)&yy_meta, sizeof(%s)},\n",
		    "YY_CHAR");

	for (i = 1; i <= numecs; ++i) {
	    if (trace)
		fprintf (stderr, "%d = %d\n",
			 i, ABS (tecbck[i]));

	    mkdata (ABS (tecbck[i]));
	    yymecs_data[i] = ABS (tecbck[i]);
	}

	dataend ();
	if (tablesext) {
	    yytbl_data_compress (yymeta_tbl);
	    if (yytbl_data_fwrite (&tableswr, yymeta_tbl) < 0)
		flexerror (_
			   ("Could not write yymeta_tbl"));
	    yytbl_data_destroy (yymeta_tbl);
	    yymeta_tbl = NULL;
	}
	/* End generating yy_meta */
    }

    total_states = lastdfa + numtemps;

    /* Begin generating yy_base */
    out_str_dec ((tblend >= INT16_MAX || long_align) ?
		 get_uint32_decl_go () : get_uint16_decl_go (),
		 "yy_base", total_states + 1);

    buf_prints (&yydmap_buf,
		"\t{YYTD_ID_BASE, (void**)&yy_base, sizeof(%s)},\n",
		(tblend >= INT16_MAX
		 || long_align) ? "flex_uint32_t" : "flex_uint16_t");
    yybase_tbl =
	(struct yytbl_data *) calloc (1,
				      sizeof (struct yytbl_data));
    yytbl_data_init (yybase_tbl, YYTD_ID_BASE);
    yybase_tbl->td_lolen = total_states + 1;
    yybase_tbl->td_data = yybase_data =
	(flex_int32_t *) calloc (yybase_tbl->td_lolen,
				 sizeof (flex_int32_t));
    yybase_curr = 1;

    for (i = 1; i <= lastdfa; ++i) {
	int d = def[i];

	if (base[i] == JAMSTATE)
	    base[i] = jambase;

	if (d == JAMSTATE)
	    def[i] = jamstate;

	else if (d < 0) {
	    /* Template reference. */
	    ++tmpuses;
	    def[i] = lastdfa - d + 1;
	}

	mkdata (base[i]);
	yybase_data[yybase_curr++] = base[i];
    }

    /* Generate jam state's base index. */
    mkdata (base[i]);
    yybase_data[yybase_curr++] = base[i];

    for (++i /* skip jam state */ ; i <= total_states; ++i) {
	mkdata (base[i]);
	yybase_data[yybase_curr++] = base[i];
	def[i] = jamstate;
    }

    dataend ();
    if (tablesext) {
	yytbl_data_compress (yybase_tbl);
	if (yytbl_data_fwrite (&tableswr, yybase_tbl) < 0)
	    flexerror (_("Could not write yybase_tbl"));
	yytbl_data_destroy (yybase_tbl);
	yybase_tbl = NULL;
    }
    /* End generating yy_base */


    /* Begin generating yy_def */
    out_str_dec ((total_states >= INT16_MAX || long_align) ?
		 get_int32_decl_go () : get_int16_decl_go (),
		 "yy_def", total_states + 1);

    buf_prints (&yydmap_buf,
		"\t{YYTD_ID_DEF, (void**)&yy_def, sizeof(%s)},\n",
		(total_states >= INT16_MAX
		 || long_align) ? "flex_int32_t" : "flex_int16_t");

    yydef_tbl =
	(struct yytbl_data *) calloc (1,
				      sizeof (struct yytbl_data));
    yytbl_data_init (yydef_tbl, YYTD_ID_DEF);
    yydef_tbl->td_lolen = total_states + 1;
    yydef_tbl->td_data = yydef_data =
	(flex_int32_t *) calloc (yydef_tbl->td_lolen, sizeof (flex_int32_t));

    for (i = 1; i <= total_states; ++i) {
	mkdata (def[i]);
	yydef_data[i] = def[i];
    }

    dataend ();
    if (tablesext) {
	yytbl_data_compress (yydef_tbl);
	if (yytbl_data_fwrite (&tableswr, yydef_tbl) < 0)
	    flexerror (_("Could not write yydef_tbl"));
	yytbl_data_destroy (yydef_tbl);
	yydef_tbl = NULL;
    }
    /* End generating yy_def */


    /* Begin generating yy_nxt */
    out_str_dec ((total_states >= INT16_MAX || long_align) ?
		 get_uint32_decl_go () : get_uint16_decl_go (), "yy_nxt",
		 tblend + 1);

    buf_prints (&yydmap_buf,
		"\t{YYTD_ID_NXT, (void**)&yy_nxt, sizeof(%s)},\n",
		(total_states >= INT16_MAX
		 || long_align) ? "flex_uint32_t" : "flex_uint16_t");

    yynxt_tbl =
	(struct yytbl_data *) calloc (1,
				      sizeof (struct yytbl_data));
    yytbl_data_init (yynxt_tbl, YYTD_ID_NXT);
    yynxt_tbl->td_lolen = tblend + 1;
    yynxt_tbl->td_data = yynxt_data =
	(flex_int32_t *) calloc (yynxt_tbl->td_lolen, sizeof (flex_int32_t));

    for (i = 1; i <= tblend; ++i) {
	/* Note, the order of the following test is important.
	 * If chk[i] is 0, then nxt[i] is undefined.
	 */
	if (chk[i] == 0 || nxt[i] == 0)
	    nxt[i] = jamstate;	/* new state is the JAM state */

	mkdata (nxt[i]);
	yynxt_data[i] = nxt[i];
    }

    dataend ();
    if (tablesext) {
	yytbl_data_compress (yynxt_tbl);
	if (yytbl_data_fwrite (&tableswr, yynxt_tbl) < 0)
	    flexerror (_("Could not write yynxt_tbl"));
	yytbl_data_destroy (yynxt_tbl);
	yynxt_tbl = NULL;
    }
    /* End generating yy_nxt */

    /* Begin generating yy_chk */
    out_str_dec ((total_states >= INT16_MAX || long_align) ?
		 get_int32_decl_go () : get_int16_decl_go (), "yy_chk",
		 tblend + 1);

    buf_prints (&yydmap_buf,
		"\t{YYTD_ID_CHK, (void**)&yy_chk, sizeof(%s)},\n",
		(total_states >= INT16_MAX
		 || long_align) ? "flex_int32_t" : "flex_int16_t");

    yychk_tbl =
	(struct yytbl_data *) calloc (1,
				      sizeof (struct yytbl_data));
    yytbl_data_init (yychk_tbl, YYTD_ID_CHK);
    yychk_tbl->td_lolen = tblend + 1;
    yychk_tbl->td_data = yychk_data =
	(flex_int32_t *) calloc (yychk_tbl->td_lolen, sizeof (flex_int32_t));

    for (i = 1; i <= tblend; ++i) {
	if (chk[i] == 0)
	    ++nummt;

	mkdata (chk[i]);
	yychk_data[i] = chk[i];
    }

    dataend ();
    if (tablesext) {
	yytbl_data_compress (yychk_tbl);
	if (yytbl_data_fwrite (&tableswr, yychk_tbl) < 0)
	    flexerror (_("Could not write yychk_tbl"));
	yytbl_data_destroy (yychk_tbl);
	yychk_tbl = NULL;
    }
    /* End generating yy_chk */

    flex_free ((void *) acc_array);
}


/* Write out a formatted string (with a secondary string argument) at the
 * current indentation level, adding a final newline.
 */

void indent_put2s_go (const char *fmt, const char *arg)
{
    do_indent_go ();
    out_str (fmt, arg);
    outn ("");
}


/* Write out a string at the current indentation level, adding a final
 * newline.
 */

void indent_puts_go (const char *str)
{
    do_indent_go ();
    outn (str);
}


/* make_tables - generate transition tables and finishes generating output file
 */

void make_tables_go (void)
{
    int i;
    int did_eof_rule = false;
    struct yytbl_data *yynultrans_tbl = NULL;


    skelout ();		/* %% [2.0] - break point in skel */

    /* First, take care of YY_DO_BEFORE_ACTION depending on yymore
     * being used.
     */
    set_indent_go (1);

    /*
    if (yymore_used && !yytext_is_array) {
	indent_puts_go ("YY_G(yytext_ptr) -= YY_G(yy_more_len); \\");
	indent_puts_go
	    ("yyleng = (size_t) (yy_cp - YY_G(yytext_ptr)); \\");
    }

    else
	indent_puts_go ("yyleng = (size_t) (yy_cp - yy_bp); \\");
    */

    /* Now also deal with copying yytext_ptr to yytext if needed. */
    skelout ();		/* %% [3.0] - break point in skel */
    if (yytext_is_array) {
	if (yymore_used)
	    indent_puts_go
		("if ( yyleng + YY_G(yy_more_offset) >= YYLMAX ) \\");
	else
	    indent_puts_go ("if ( yyleng >= YYLMAX ) \\");

	indent_up_go ();
	indent_puts_go
	    ("YY_FATAL_ERROR( \"token too large, exceeds YYLMAX\" ); \\");
	indent_down_go ();

	if (yymore_used) {
	    indent_puts_go
		("yy_flex_strncpy( &yytext[YY_G(yy_more_offset)], YY_G(yytext_ptr), yyleng + 1 M4_YY_CALL_LAST_ARG); \\");
	    indent_puts_go ("yyleng += YY_G(yy_more_offset); \\");
	    indent_puts_go
		("YY_G(yy_prev_more_offset) = YY_G(yy_more_offset); \\");
	    indent_puts_go ("YY_G(yy_more_offset) = 0; \\");
	}
	else {
	    indent_puts_go
		("yy_flex_strncpy( yytext, YY_G(yytext_ptr), yyleng + 1 M4_YY_CALL_LAST_ARG); \\");
	}
    }

    set_indent_go (0);

    skelout ();		/* %% [4.0] - break point in skel */


    /* This is where we REALLY begin generating the tables. */

    out_dec (Go ? "const yy_NUM_RULES = %d\n" : "#define YY_NUM_RULES %d\n", num_rules);
    out_dec (Go ? "const yy_END_OF_BUFFER = %d\n" : "#define YY_END_OF_BUFFER %d\n", num_rules + 1);

    if (fullspd) {
	/* Need to define the transet type as a size large
	 * enough to hold the biggest offset.
	 */
	int     total_table_size = tblend + numecs + 1;
	char   *trans_offset_type =
	    (total_table_size >= INT16_MAX || long_align) ?
	    "flex_int32_t" : "flex_int16_t";

	set_indent_go (0);
	indent_puts_go ("struct yy_trans_info");
	indent_up_go ();
	indent_puts_go ("{");

	/* We require that yy_verify and yy_nxt must be of the same size int. */
	indent_put2s_go ("%s yy_verify;", trans_offset_type);

	/* In cases where its sister yy_verify *is* a "yes, there is
	 * a transition", yy_nxt is the offset (in records) to the
	 * next state.  In most cases where there is no transition,
	 * the value of yy_nxt is irrelevant.  If yy_nxt is the -1th
	 * record of a state, though, then yy_nxt is the action number
	 * for that state.
	 */

	indent_put2s_go ("%s yy_nxt;", trans_offset_type);
	indent_puts_go ("};");
	indent_down_go ();
    }
    else {
	/* We generate a bogus 'struct yy_trans_info' data type
	 * so we can guarantee that it is always declared in the skel.
	 * This is so we can compile "sizeof(struct yy_trans_info)"
	 * in any scanner.
	 */
	indent_puts_go
	    ("/* This struct is not used in this scanner,");
	indent_puts_go ("   but its presence is necessary. */");

	indent_puts_go("type yy_trans_info struct {");
	indent_up_go();
	indent_puts_go ("yy_verify int32");
	indent_puts_go ("yy_nxt    int32");
	indent_puts_go ("}");

	indent_down_go ();
    }

    if (fullspd) {
	genctbl_go ();
	if (tablesext) {
	    struct yytbl_data *tbl;

	    tbl = mkctbl_go ();
	    yytbl_data_compress (tbl);
	    if (yytbl_data_fwrite (&tableswr, tbl) < 0)
		flexerror (_("Could not write ftbl"));
	    yytbl_data_destroy (tbl);

	    tbl = mkssltbl_go ();
	    yytbl_data_compress (tbl);
	    if (yytbl_data_fwrite (&tableswr, tbl) < 0)
		flexerror (_("Could not write ssltbl"));
	    yytbl_data_destroy (tbl);
	    tbl = 0;

	    if (useecs) {
		tbl = mkecstbl ();
		yytbl_data_compress (tbl);
		if (yytbl_data_fwrite (&tableswr, tbl) < 0)
		    flexerror (_
			       ("Could not write ecstbl"));
		yytbl_data_destroy (tbl);
		tbl = 0;
	    }
	}
    }
    else if (fulltbl) {
	genftbl_go ();
	if (tablesext) {
	    struct yytbl_data *tbl;

	    tbl = mkftbl ();
	    yytbl_data_compress (tbl);
	    if (yytbl_data_fwrite (&tableswr, tbl) < 0)
		flexerror (_("Could not write ftbl"));
	    yytbl_data_destroy (tbl);
	    tbl = 0;

	    if (useecs) {
		tbl = mkecstbl ();
		yytbl_data_compress (tbl);
		if (yytbl_data_fwrite (&tableswr, tbl) < 0)
		    flexerror (_
			       ("Could not write ecstbl"));
		yytbl_data_destroy (tbl);
		tbl = 0;
	    }
	}
    }
    else
	gentabs_go ();

    if (do_yylineno) {

	geneoltbl_go ();

	if (tablesext) {
	    struct yytbl_data *tbl;

	    tbl = mkeoltbl_go ();
	    yytbl_data_compress (tbl);
	    if (yytbl_data_fwrite (&tableswr, tbl) < 0)
		flexerror (_("Could not write eoltbl"));
	    yytbl_data_destroy (tbl);
	    tbl = 0;
	}
    }

    /* Definitions for backing up.  We don't need them if REJECT
     * is being used because then we use an alternative backin-up
     * technique instead.
     */
    if (num_backing_up > 0 && !reject) {
	if (!C_plus_plus && !reentrant) {
	    indent_puts_go
		("var yy_last_accepting_state int");
	    indent_puts_go
		("var yy_last_accepting_cpos int");
	}
    }

    if (nultrans) {
	flex_int32_t *yynultrans_data = 0;

	/* Begin generating yy_NUL_trans */
	out_str_dec (get_state_decl_go (), "yy_NUL_trans",
		     lastdfa + 1);
	buf_prints (&yydmap_buf,
		    "\t{YYTD_ID_NUL_TRANS, (void**)&yy_NUL_trans, sizeof(%s)},\n",
		    (fullspd) ? "struct yy_trans_info*" :
		    "flex_int32_t");

	yynultrans_tbl =
	    (struct yytbl_data *) calloc (1,
					  sizeof (struct
						  yytbl_data));
	yytbl_data_init (yynultrans_tbl, YYTD_ID_NUL_TRANS);
	if (fullspd)
	    yynultrans_tbl->td_flags |= YYTD_PTRANS;
	yynultrans_tbl->td_lolen = lastdfa + 1;
	yynultrans_tbl->td_data = yynultrans_data =
	    (flex_int32_t *) calloc (yynultrans_tbl->td_lolen,
				     sizeof (flex_int32_t));

	for (i = 1; i <= lastdfa; ++i) {
	    if (fullspd) {
		out_dec ("    &yy_transition[%d],\n",
			 base[i]);
		yynultrans_data[i] = base[i];
	    }
	    else {
		mkdata (nultrans[i]);
		yynultrans_data[i] = nultrans[i];
	    }
	}

	dataend ();
	if (tablesext) {
	    yytbl_data_compress (yynultrans_tbl);
	    if (yytbl_data_fwrite (&tableswr, yynultrans_tbl) <
		0)
		flexerror (_
			   ("Could not write yynultrans_tbl"));
	}

	if (yynultrans_tbl != NULL) {
	    yytbl_data_destroy (yynultrans_tbl);
	    yynultrans_tbl = NULL;
        }

	/* End generating yy_NUL_trans */
    }

    if (!C_plus_plus && !reentrant) {
	indent_put2s_go ("var yy_flex_debug = %s\n",
			 ddebug ? "true" : "false");
    }

    if (ddebug) {		/* Spit out table mapping rules to line numbers. */
	out_str_dec (long_align ? get_int32_decl_go () :
		     get_int16_decl_go (), "yy_rule_linenum",
		     num_rules);
	for (i = 1; i < num_rules; ++i)
	    mkdata (rule_linenum[i]);
	dataend ();
    }

    if (reject) {
	outn ("m4_ifdef( [[M4_YY_USES_REJECT]],\n[[");
	/* Declare state buffer variables. */
	if (!C_plus_plus && !reentrant) {
	    outn ("static yy_state_type *yy_state_buf=0, *yy_state_ptr=0;");
	    outn ("static char *yy_full_match;");
	    outn ("static int yy_lp;");
	}

	if (variable_trailing_context_rules) {
	    if (!C_plus_plus && !reentrant) {
		outn ("static int yy_looking_for_trail_begin = 0;");
		outn ("static int yy_full_lp;");
		outn ("static int *yy_full_state;");
	    }

	    out_hex ("#define YY_TRAILING_MASK 0x%x\n",
		     (unsigned int) YY_TRAILING_MASK);
	    out_hex ("#define YY_TRAILING_HEAD_MASK 0x%x\n",
		     (unsigned int) YY_TRAILING_HEAD_MASK);
	}

	outn ("#define REJECT \\");
	outn ("{ \\");
	outn ("*yy_cp = YY_G(yy_hold_char); /* undo effects of setting up yytext */ \\");
	outn ("yy_cp = YY_G(yy_full_match); /* restore poss. backed-over text */ \\");

	if (variable_trailing_context_rules) {
	    outn ("YY_G(yy_lp) = YY_G(yy_full_lp); /* restore orig. accepting pos. */ \\");
	    outn ("YY_G(yy_state_ptr) = YY_G(yy_full_state); /* restore orig. state */ \\");
	    outn ("yy_current_state = *YY_G(yy_state_ptr); /* restore curr. state */ \\");
	}

	outn ("++YY_G(yy_lp); \\");
	outn ("goto find_rule; \\");

	outn ("}");
	outn ("]])\n");
    }

    else {
	if (Go) {
	    // TODO
	} else {
	    outn ("/* The intent behind this definition is that it'll catch");
	    outn (" * any uses of REJECT which flex missed.");
	    outn (" */");
	    outn ("#define REJECT reject_used_but_not_detected");
	}
    }

    if (yymore_used) {
	if (!C_plus_plus) {
	    if (yytext_is_array) {
		if (!reentrant){
		    indent_puts_go ("static int yy_more_offset = 0;");
                    indent_puts_go ("static int yy_prev_more_offset = 0;");
                }
	    }
	    else if (!reentrant) {
		indent_puts_go
		    ("static int yy_more_flag = 0;");
		indent_puts_go
		    ("static int yy_more_len = 0;");
	    }
	}

	if (yytext_is_array) {
	    indent_puts_go
		("#define yymore() (YY_G(yy_more_offset) = yy_flex_strlen( yytext M4_YY_CALL_LAST_ARG))");
	    indent_puts_go ("#define YY_NEED_STRLEN");
	    indent_puts_go ("#define YY_MORE_ADJ 0");
	    indent_puts_go
		("#define YY_RESTORE_YY_MORE_OFFSET \\");
	    indent_up_go ();
	    indent_puts_go ("{ \\");
	    indent_puts_go
		("YY_G(yy_more_offset) = YY_G(yy_prev_more_offset); \\");
	    indent_puts_go ("yyleng -= YY_G(yy_more_offset); \\");
	    indent_puts_go ("}");
	    indent_down_go ();
	}
	else {
	    indent_puts_go
		("#define yymore() (YY_G(yy_more_flag) = 1)");
	    indent_puts_go
		("#define YY_MORE_ADJ YY_G(yy_more_len)");
	    indent_puts_go ("#define YY_RESTORE_YY_MORE_OFFSET");
	}
    }

    else {
	if (Go) {
	    // TODO
	} else {
	    indent_puts_go
		("#define yymore() yymore_used_but_not_detected");
	    indent_puts_go ("#define YY_MORE_ADJ 0");
	    indent_puts_go ("#define YY_RESTORE_YY_MORE_OFFSET");
	}
    }

    if (!C_plus_plus) {
	if (yytext_is_array) {
	    outn ("#ifndef YYLMAX");
	    outn ("#define YYLMAX 8192");
	    outn ("#endif\n");
	    if (!reentrant){
                outn ("char yytext[YYLMAX];");
                outn ("char *yytext_ptr;");
            }
	}

	else {
	    if(! reentrant)
		outn (Go ? "var YYtext []byte" : "char *yytext;");
	}
    }

    if (!Go)
	out (&action_array[defs1_offset]);

    line_directive_out (stdout, 0);

    skelout ();		/* %% [5.0] - break point in skel */

    skelout ();		/* %% [6.0] - break point in skel */

    indent_puts_go ("func YY_RULE_SETUP() {");
    indent_up_go ();
    if (bol_needed) {
	indent_puts_go ("buffer.yy_at_bol = strings.HasSuffix(YYtext, \"\\n\")\n");
    }
    indent_puts_go ("//YY_USER_ACTION()");
    indent_down_go ();
    indent_puts_go ("}");

    skelout ();		/* %% [7.0] - break point in skel */

    /* Copy prolog to output file. */
    out (&action_array[prolog_offset]);

    line_directive_out (stdout, 0);

    skelout ();		/* %% [8.0] - break point in skel */

    set_indent_go (2);

    if (yymore_used && !yytext_is_array) {
	indent_puts_go ("YY_G(yy_more_len) = 0;");
	indent_puts_go ("if ( YY_G(yy_more_flag) )");
	indent_up_go ();
	indent_puts_go ("{");
	indent_puts_go
	    ("YY_G(yy_more_len) = YY_G(yy_c_buf_p) - YY_G(yytext_ptr);");
	indent_puts_go ("YY_G(yy_more_flag) = 0;");
	indent_puts_go ("}");
	indent_down_go ();
    }

    skelout ();		/* %% [9.0] - break point in skel */

    gen_start_state_go ();

    /* Note, don't use any indentation. */
    outn ("yy_match:");
    indent_puts_go ("buffer = yy_buffer_stack[yy_buffer_stack_top]");
    gen_next_match_go ();

    skelout ();		/* %% [10.0] - break point in skel */
    set_indent_go (2);
    gen_find_action_go ();

    skelout ();		/* %% [11.0] - break point in skel */
    outn ("m4_ifdef( [[M4_YY_USE_LINENO]],[[");
    indent_puts_go
	("if ( yy_act != YY_END_OF_BUFFER && yy_rule_can_match_eol[yy_act] )");
    indent_up_go ();
    indent_puts_go ("{");
    indent_puts_go ("yy_size_t yyl;");
    do_indent_go ();
    out_str ("for ( yyl = %s; yyl < yyleng; ++yyl )\n",
	     yymore_used ? (yytext_is_array ? "YY_G(yy_prev_more_offset)" :
			    "YY_G(yy_more_len)") : "0");
    indent_up_go ();
    indent_puts_go ("if ( yytext[yyl] == '\\n' )");
    indent_up_go ();
    indent_puts_go ("M4_YY_INCR_LINENO();");
    indent_down_go ();
    indent_down_go ();
    indent_puts_go ("}");
    indent_down_go ();
    outn ("]])");

    skelout ();		/* %% [12.0] - break point in skel */
    if (ddebug) {
	indent_puts_go ("if yy_flex_debug {");
	indent_up_go ();

	indent_puts_go ("if yy_act == 0 {");
	indent_up_go ();
	indent_puts_go ("fmt.Fprintln(os.Stderr, \"--scanner backing up\")");
	indent_down_go ();

	do_indent_go ();
	out_dec ("} else if yy_act < %d {\n", num_rules);
	indent_up_go ();

	indent_puts_go ("fmt.Fprintf(os.Stderr, \"--accepting rule at line %d (%q)\\n\",");
	indent_puts_go ("         yy_rule_linenum[yy_act], YYtext)");

	indent_down_go ();

	do_indent_go ();
	out_dec ("} else if yy_act == %d {\n", num_rules);
	indent_up_go ();

	indent_puts_go ("fmt.Fprintf(os.Stderr, \"--accepting default rule (%q)\\n\", YYtext)");

	indent_down_go ();

	do_indent_go ();
	out_dec ("} else if yy_act == %d {\n", num_rules + 1);
	indent_up_go ();

	indent_puts_go ("fmt.Fprintln(os.Stderr, \"--(end of buffer or a NUL)\")");

	indent_down_go ();

	do_indent_go ();
	outn ("} else {");
	indent_up_go ();

	indent_puts_go
	    ("fmt.Fprintf(os.Stderr, \"--EOF (start condition %d)\\n\", yy_START)");

	indent_down_go ();
	indent_puts_go ("}");

	indent_down_go ();
	indent_puts_go ("}");
    }

    /* Copy actions to output file. */
    skelout ();		/* %% [13.0] - break point in skel */
    indent_up_go ();
    gen_bu_action_go ();
    out (&action_array[action_offset]);

    line_directive_out (stdout, 0);

    /* generate cases for any missing EOF rules */
    for (i = 1; i <= lastsc; ++i)
	if (!sceof[i]) {
	    do_indent_go ();
	    out_str ("case yy_STATE_EOF(%s):\n", scname[i]);
	    did_eof_rule = true;
	}

    if (did_eof_rule) {
	indent_up_go ();
	indent_puts_go ("YYterminate()");
	indent_down_go ();
    }


    /* Generate code for handling NUL's, if needed. */

    /* First, deal with backing up and setting up yy_cp if the scanner
     * finds that it should JAM on the NUL.
     */
    skelout ();		/* %% [14.0] - break point in skel */
    set_indent_go (4);

    if (fullspd || fulltbl)
	indent_puts_go ("yy_cp = YY_G(yy_c_buf_p);");

    else {			/* compressed table */
	if (!reject && !interactive) {
	    /* Do the guaranteed-needed backing up to figure
	     * out the match.
	     */

	    indent_puts_go ("yy_cp = yy_last_accepting_cpos");
	    indent_puts_go ("yy_current_state = yy_last_accepting_state");

	}

	else {
	    /* Still need to initialize yy_cp, though
	     * yy_current_state was set up by
	     * yy_get_previous_state().
	     */
	    // TODO
	    //indent_puts_go ("yy_cp = YY_G(yy_c_buf_p);");
	}
    }


    /* Generate code for yy_get_previous_state(). */
    set_indent_go (1);
    skelout ();		/* %% [15.0] - break point in skel */

    gen_start_state_go ();

    set_indent_go (2);
    skelout ();		/* %% [16.0] - break point in skel */
    gen_next_state_go (true);

    set_indent_go (1);
    skelout ();		/* %% [17.0] - break point in skel */
    gen_NUL_trans_go ();

    skelout ();		/* %% [18.0] - break point in skel */
    skelout ();		/* %% [19.0] - break point in skel */
    /* Update BOL and yylineno inside of input(). */
    if (bol_needed) {
	indent_puts_go
	    ("YY_CURRENT_BUFFER_LVALUE->yy_at_bol = (c == '\\n');");
	if (do_yylineno) {
	    indent_puts_go
		("if ( YY_CURRENT_BUFFER_LVALUE->yy_at_bol )");
	    indent_up_go ();
	    indent_puts_go ("M4_YY_INCR_LINENO();");
	    indent_down_go ();
	}
    }

    else if (do_yylineno) {
	indent_puts_go ("if ( c == '\\n' )");
	indent_up_go ();
	indent_puts_go ("M4_YY_INCR_LINENO();");
	indent_down_go ();
    }

    skelout ();

    /* Copy remainder of input to output. */

    line_directive_out (stdout, 1);

    if (sectnum == 3) {
	OUT_BEGIN_CODE ();
	(void) flexscan ();	/* copy remainder of input to output */
	OUT_END_CODE ();
    }
}
