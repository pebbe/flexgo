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

    do_indent_go ();
    if (fullspd)
	indent_puts_go ("if yyTransition[yy.currentState-1].yyNxt != 0 {");
    else
	indent_puts_go ("if yyAccept[yy.currentState] != 0 {");

    indent_up_go ();
    indent_puts_go ("yy.lastAcceptingState = yy.currentState");
    indent_puts_go ("yy.lastAcceptingCpos = yy.cp");
    indent_down_go ();
    indent_puts_go ("}");
}


/* Generate the code to perform the backing up. */

void gen_bu_action_go (void)
{
    if (reject || num_backing_up == 0)
	return;

    set_indent_go (3);

    indent_puts_go ("case 0: // must back up");
    indent_puts_go ("// undo the effects of yy_DO_BEFORE_ACTION");
    indent_puts_go ("yy.chBuf[yy.cp] = yy.holdChar");

    if (fullspd || fulltbl)
	indent_puts_go ("yy.cp = yy.lastAcceptingCpos + 1");
    else
	/* Backing-up info for compressed tables is taken \after/
	 * yy_cp has been incremented for the next state.
	 */
	indent_puts_go ("yy.cp = yy.lastAcceptingCpos");

    indent_puts_go ("yy.currentState = yy.lastAcceptingState");
    indent_puts_go ("goto yyFindAction");
    outc ('\n');

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
	out_dec ("var yyTransition = [%d]yyTransInfo{\n", tblend + numecs + 1);
    else
	outn ("var yyTransition []yyTransInfo");

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
	out_dec ("var yyStartStateList = [%d]int{\n", lastsc * 2 + 1);
    else
	outn ("var yyStartStateList []int\n");

    if (gentables) {

	for (i = 0; i <= lastsc * 2; ++i)
	    out_dec ("%d,\n", base[i]);

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

    out_str_dec (get_yy_char_decl_go (), "yyEc", csize);

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
    if (fullspd) {
	indent_puts_go ("yy.act = int(yyTransition[yy.currentState-1].yyNxt)");
    }

    else if (fulltbl) {
	indent_puts_go ("yy.act = int(yyAccept[yy.currentState])");
    }

    else if (reject) {
	indent_puts_go ("sp := len(yy.stateBuf) - 1");
	indent_puts_go ("yy.currentState = yy.stateBuf[sp]");
	indent_puts_go ("yy.stateBuf = yy.stateBuf[:sp]");
	indent_puts_go ("yy.lp = int(yyAccept[yy.currentState])");

	indent_puts_go ("findRule: // we branch to this label when backing up");

	indent_puts_go ("for { // until we find what rule we matched");
	indent_up_go ();
	indent_puts_go ("if yy.lp != 0 && yy.lp < int(yyAccept[yy.currentState+1]) {");
	indent_up_go ();
	indent_puts_go ("yy.act = int(yyAcclist[yy.lp])");

	if (variable_trailing_context_rules) {
	    indent_puts_go
		("if yy.act & yyTrailingHeadMask != 0 ||");
	    indent_puts_go ("     yy.lookingForTrailBegin != 0 {");
	    indent_up_go ();

	    indent_puts_go
		("if yy.act == yy.lookingForTrailBegin {");
	    indent_up_go ();
	    indent_puts_go ("yy.lookingForTrailBegin = 0");
	    indent_puts_go ("yy.act &= ^yyTrailingHeadMask");
	    indent_puts_go ("break");
	    indent_puts_go ("}");
	    indent_down_go ();

	    indent_down_go ();

	    indent_puts_go
		("} else if yy.act & yyTrailingMask != 0 {");
	    indent_up_go ();
	    indent_puts_go
		("yy.lookingForTrailBegin = yy.act & ^yyTrailingMask");
	    indent_puts_go
		("yy.lookingForTrailBegin |= yyTrailingHeadMask");

	    if (real_reject) {
		/* Remember matched text in case we back up
		 * due to REJECT.
		 */
		indent_puts_go
		    ("yy.fullMatch = yy.cp");
		indent_puts_go
		    ("yy.fullState = yy.statePtr");
		indent_puts_go ("yy.fullLp = yy.lp");
	    }

	    indent_down_go ();

	    indent_puts_go ("} else {");
	    indent_up_go ();
	    indent_puts_go ("yy.fullMatch = yy.cp");
	    indent_puts_go
		("yy.fullState = yy.statePtr");
	    indent_puts_go ("yy.fullLp = yy.lp");
	    indent_puts_go ("break");
	    indent_down_go ();
	    indent_puts_go ("}");

	    indent_puts_go ("yy.lp++");
	    indent_puts_go ("goto findRule");
	}

	else {
	    /* Remember matched text in case we back up due to
	     * trailing context plus REJECT.
	     */
	    indent_up_go ();
	    indent_puts_go ("{");
	    indent_puts_go ("yy.fullMatch = yy.cp");
	    indent_puts_go ("break");
	    indent_puts_go ("}");
	    indent_down_go ();
	}

	indent_puts_go ("}");
	indent_down_go ();

	indent_puts_go ("yy.cp--");

	/* We could consolidate the following two lines with those at
	 * the beginning, but at the cost of complaints that we're
	 * branching inside a loop.
	 */
	indent_puts_go ("sp := len(yy.stateBuf) - 1");
	indent_puts_go ("yy.currentState = yy.stateBuf[sp]");
	indent_puts_go ("yy.stateBuf = yy.stateBuf[:sp]");
	indent_puts_go ("yy.lp = int(yyAccept[yy.currentState])");

	indent_down_go ();
	indent_puts_go ("}");
    }

    else {			/* compressed */
	indent_puts_go ("yy.act = int(yyAccept[yy.currentState])");

	if (interactive && !reject) {
	    /* Do the guaranteed-needed backing up to figure out
	     * the match.
	     */
	    indent_puts_go ("if yy.act == 0 {");
	    indent_up_go ();
	    indent_puts_go ("// have to back up");
	    indent_puts_go ("yy.cp = yy.lastAcceptingCpos");
	    indent_puts_go ("yy.currentState = yy.lastAcceptingState");
	    indent_puts_go ("yy.act = int(yyAccept[yy.currentState])");
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
		 "yyAccept", lastdfa + 1);

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
    indent_put2s_go ("yyC := %s", char_map);

    /* Save the backing-up info \before/ computing the next state
     * because we always compute one more state than needed - we
     * always proceed until we reach a jam state
     */
    gen_backing_up_go ();

    indent_puts_go
	("for int(yyChk[int(yyBase[yy.currentState])+yyC]) != yy.currentState {");
    indent_up_go ();
    indent_puts_go ("yy.currentState = int(yyDef[yy.currentState])");

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
	out_dec ("if yy.currentState >= %d {\n", lastdfa + 2);

	indent_up_go ();
	indent_puts_go ("yyC = int(yyMeta[yyC])");
	indent_down_go ();
	indent_puts_go ("}");
    }

    indent_down_go ();
    indent_puts_go ("}");

    indent_puts_go
	("yy.currentState = int(yyNxt[int(yyBase[yy.currentState])+yyC])");
}


/* Generate the code to find the next match. */

void gen_next_match_go (void)
{
    /* NOTE - changes in here should be reflected in gen_next_state_go() and
     * gen_NUL_trans().
     */
    char   *char_map = useecs ?
	"yyEc[yy.chBuf[yy.cp]]" : "yy.chBuf[yy.cp]";

    char   *char_map_2 = useecs ?
	"yy_ec[YY_SC_TO_UI(*++yy_cp)] " : "YY_SC_TO_UI(*++yy_cp)";

    if (fulltbl) {
	if (gentables) {
	    indent_puts_go ("for {");
	    indent_up_go ();
	    indent_put2s_go (
			     "yy.currentState = int(yyNxt[yy.currentState][%s])",
			     char_map);
	    indent_puts_go ("if yy.currentState <= 0 {");
	    indent_up_go ();
	    indent_puts_go ("break");
	    indent_down_go ();
	    indent_puts_go ("}");
	    indent_down_go ();
	}
	else
	    indent_put2s_go
		("while ( (yy.currentState = yy_nxt[yy.currentState*YY_NXT_LOLEN +  %s ]) > 0 )",
		 char_map);

	indent_up_go ();

	if (num_backing_up > 0) {
		    gen_backing_up_go ();
	    outc ('\n');
	}

	indent_puts_go ("yy.cp++");

	indent_down_go ();
	indent_puts_go ("}");


	outc ('\n');
	indent_puts_go ("yy.currentState = -yy.currentState");
    }

    else if (fullspd) {

	indent_puts_go ("yyC := yy.chBuf[yy.cp]");
	indent_puts_go ("for {");
	indent_up_go ();
	indent_puts_go ("transInfo := yyTransition[yy.currentState+int(yyC)]");
	indent_puts_go ("if int(transInfo.yyVerify) != int(yyC) {");
	indent_up_go ();
	indent_puts_go ("break");
	indent_down_go ();
	indent_puts_go ("}");
	indent_puts_go ("yy.cp++");
	indent_puts_go ("yyC = yy.chBuf[yy.cp]");

	indent_puts_go ("yy.currentState += int(transInfo.yyNxt)");

	if (num_backing_up > 0)
	    gen_backing_up_go ();

	indent_down_go ();
	indent_puts_go ("}");
    }

    else {			/* compressed */

	indent_puts_go( "for {");

	indent_up_go ();

	gen_next_state_go (false);

	indent_puts_go( "yy.cp++");

	do_indent_go ();

	if (interactive)
	    out_dec ("if yyBase[yy.currentState] == %d {\n", jambase);
	else
	    out_dec ("if yy.currentState == %d {\n",
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

	    indent_puts_go ("yy.cp = yy.lastAcceptingCpos");
	    indent_puts_go ("yy.currentState = yy.lastAcceptingState");

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
		      "yyIfElse(yy.chBuf[yy.cp] != 0, int(yyEc[yy.chBuf[yy.cp]]), %d)",
		      NUL_ec);
	else
            snprintf (char_map, sizeof(char_map),
		      "yyIfElse(yy.chBuf[yy.cp] != 0, int(yy.chBuf[yy.cp]), %d)",
		      NUL_ec);
    }

    else
	strcpy (char_map, useecs ?
		"int(yyEc[yy.chBuf[yy.cp]])" :
		"int(yy.chBuf[yy.cp])");

    if (worry_about_NULs && nultrans) {
	if (!fulltbl && !fullspd)
	    /* Compressed tables back up *before* they match. */
	    gen_backing_up_go ();

	indent_puts_go ("if yy.chBuf[yy.cp] != 0 {");
	indent_up_go ();
    }

    if (fulltbl) {
	if (gentables)
	    indent_put2s_go
		("yy.currentState = int(yyNxt[yy.currentState][%s])",
		 char_map);
	else
	    indent_put2s_go
		("yy.currentState = int(yyNxt[yy.currentState*YY_NXT_LOLEN + %s])",
		 char_map);
    }

    else if (fullspd)
	indent_put2s_go
	    ("yy.currentState += int(yyTransition[yy.currentState+%s].yyNxt)",
	     char_map);

    else
	gen_next_compressed_state_go (char_map);

    if (worry_about_NULs && nultrans) {

	indent_puts_go ("} else {");
	indent_up_go ();
	indent_puts_go
	    ("yy.currentState = int(yyNulTrans[yy.currentState])");
	indent_down_go ();
	indent_puts_go ("}");
    }

    if (fullspd || fulltbl)
	gen_backing_up_go ();

    if (reject)
	indent_puts_go ("yy.stateBuf = append(yy.stateBuf, yy.currentState)");
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
	indent_puts_go ("yy.cp = yy.cBufP");
    }

    outc ('\n');

    if (nultrans) {
	indent_puts_go
	    ("yyCurrentState = int(yyNulTrans[yyCurrentState])");
	indent_puts_go ("yyIsJam = (yyCurrentState == 0)");
    }

    else if (fulltbl) {
	do_indent_go ();
	if (gentables)
	    out_dec ("yy.currentState = yy_nxt[yy.currentState][%d];\n", NUL_ec);
	else
	    out_dec ("yy.currentState = yy_nxt[yy.currentState*YY_NXT_LOLEN + %d];\n", NUL_ec);
	indent_puts_go ("yyIsJam = (yy.currentState <= 0)");
    }

    else if (fullspd) {
	do_indent_go ();
	out_dec ("yyC := %d\n", NUL_ec);

	indent_puts_go
	    ("transInfo := yyTransition[yy.currentState+int(yyC)]");
	indent_puts_go ("yy.currentState += int(transInfo.yyNxt)");

	indent_puts_go
	    ("yyIsJam = int(transInfo.yyVerify) != int(yyC)");
    }

    else {
	char    NUL_ec_str[20];

	snprintf (NUL_ec_str, sizeof(NUL_ec_str), "%d", NUL_ec);
	gen_next_compressed_state_go (NUL_ec_str);

	do_indent_go ();
	out_dec ("if yy.currentState == %d {\n", jamstate);
	indent_up_go ();
	indent_puts_go ("yyIsJam = true");
	indent_down_go ();
	indent_puts_go ("}");

	if (reject) {
	    /* Only stack this state if it's a transition we
	     * actually make.  If we stack it on a jam, then
	     * the state stack and yy_c_buf_p get out of sync.
	     */
	    indent_puts_go ("if ! yyIsJam {");
	    indent_up_go ();
	    indent_puts_go
		("yy.stateBuf = append(yy.stateBuf, yyCurrentState)");
	    indent_down_go ();
	    indent_puts_go ("}");
	}
    }

    /* If we've entered an accepting state, back up; note that
     * compressed tables have *already* done such backing up, so
     * we needn't bother with it again.
     */
    if (need_backing_up && (fullspd || fulltbl)) {
	outc ('\n');
	indent_puts_go ("if !yyIsJam {");
	indent_up_go ();
	gen_backing_up_go ();
	indent_down_go ();
	indent_puts_go ("}");
    }
}


/* Generate the code to find the start state. */

void gen_start_state_go (void)
{
    if (fullspd) {
	if (bol_needed) {
	    indent_puts_go
		("yy.currentState = yyStartStateList[yy.start + yy.atBol]");
	}
	else
	    indent_puts_go
		("yy.currentState = yyStartStateList[yy.start]");
    }

    else {
	indent_puts_go ("yy.currentState = yy.start");

	if (bol_needed)
	    indent_puts_go ("yy.currentState += yy.atBol");

	if (reject) {
	    /* Set up for storing up states. */
	    outn ("m4_ifdef( [[M4_YY_USES_REJECT]],\n[[");
	    indent_puts_go
		("yy.stateBuf = yy.stateBuf[0:0]");
	    indent_puts_go
		("yy.stateBuf = append(yy.stateBuf, yy.currentState)");
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
		     get_int16_decl_go (), "yyAcclist", MAX (numas,
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
		 "yyAccept", k);

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

	out_str_dec (get_yy_char_decl_go (), "yyMeta", numecs + 1);
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
		 "yyBase", total_states + 1);

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
		 "yyDef", total_states + 1);

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
		 get_uint32_decl_go () : get_uint16_decl_go (), "yyNxt",
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
		 get_int32_decl_go () : get_int16_decl_go (), "yyChk",
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
    struct yytbl_data *yynultrans_tbl = NULL;


    skelout ();		/* %% [2.0] - break point in skel */

    /* First, take care of yy_DO_BEFORE_ACTION depending on yymore
     * being used.
     */
    set_indent_go (1);

    if (yymore_used) {
	indent_puts_go ("yy.textPtr -= yy.moreLen");
	indent_puts_go
	    ("yy.Leng = yy.cp - yy.textPtr");
    }

    else
	indent_puts_go ("yy.Leng = yy.cp - yy.bp");

    /* Now also deal with copying yytext_ptr to yytext if needed. */
    skelout ();		/* %% [3.0] - break point in skel */
    if (yytext_is_array) {
	if (yymore_used)
	    indent_puts_go
		("if yyleng + yy_more_offset >= YYLMAX \\");
	else
	    indent_puts_go ("if yyleng >= YYLMAX \\");

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

    out_dec ("const yyNumRules = %d\n", num_rules);
    out_dec ("const yyEndOfBuffer = %d\n", num_rules + 1);

    if (fullspd) {
	/* Need to define the transet type as a size large
	 * enough to hold the biggest offset.
	 */
	int     total_table_size = tblend + numecs + 1;
	char   *trans_offset_type =
	    (total_table_size >= INT16_MAX || long_align) ?
	    "int32" : "int16";

	set_indent_go (0);
	indent_puts_go ("type yyTransInfo struct {");
	indent_up_go ();

	/* We require that yy_verify and yy_nxt must be of the same size int. */
	indent_put2s_go ("yyVerify %s", trans_offset_type);

	/* In cases where its sister yy_verify *is* a "yes, there is
	 * a transition", yy_nxt is the offset (in records) to the
	 * next state.  In most cases where there is no transition,
	 * the value of yy_nxt is irrelevant.  If yy_nxt is the -1th
	 * record of a state, though, then yy_nxt is the action number
	 * for that state.
	 */

	indent_put2s_go ("yyNxt %s", trans_offset_type);
	indent_puts_go ("}");
	indent_down_go ();
    }
    else {
	/* We generate a bogus 'struct yy_trans_info' data type
	 * so we can guarantee that it is always declared in the skel.
	 * This is so we can compile "sizeof(struct yy_trans_info)"
	 * in any scanner.
	 */
	if (0) {
	    indent_puts_go
		("/* This struct is not used in this scanner,");
	    indent_puts_go ("   but its presence is necessary. */");

	    indent_puts_go("type yyTransInfo struct {");
	    indent_up_go();
	    indent_puts_go ("yyVerify int32");
	    indent_puts_go ("yyNxt    int32");
	    indent_puts_go ("}");

	    indent_down_go ();
	}
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

    if (nultrans) {
	flex_int32_t *yynultrans_data = 0;

	/* Begin generating yy_NUL_trans */
	out_str_dec (get_state_decl_go (), "yyNulTrans",
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

    if (ddebug) {		/* Spit out table mapping rules to line numbers. */
	out_str_dec (long_align ? get_int32_decl_go () :
		     get_int16_decl_go (), "yyRuleLinenum",
		     num_rules);
	for (i = 1; i < num_rules; ++i)
	    mkdata (rule_linenum[i]);
	dataend ();
    }

    if (reject) {
	outn ("m4_ifdef( [[M4_YY_USES_REJECT]],\n[[");

	if (variable_trailing_context_rules) {

	    out_hex ("const yyTrailingMask = 0x%x\n",
		     (unsigned int) YY_TRAILING_MASK);
	    out_hex ("const yyTrailingHeadMask = 0x%x\n",
		     (unsigned int) YY_TRAILING_HEAD_MASK);
	}

	outn ("m4_define( [[REJECT]], [[");
       	outn ("yy.chBuf[yy.cp] = yy.holdChar // undo effects of setting up yytext");
	outn ("yy.cp = yy.fullMatch          // restore poss. backed-over text");

	if (variable_trailing_context_rules) {
	    outn ("yy.lp = yy.fullLp // restore orig. accepting pos.");
	    outn ("yy.statePtr = yy.fullState // restore orig. state");
	    outn ("yy.currentState = yy.statePtr // restore curr. state");
	}

       	outn ("yy.lp++");
       	outn ("goto findRule");
	outn ("]])");

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
		    indent_puts_go ("static int yy_more_offset = 0");
                    indent_puts_go ("static int yy_prev_more_offset = 0");
                }
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
		("m4_define( [[yymore]], [[yy.moreFlag = true]] )");
	    indent_puts_go
		("m4_define( [[YY_MORE_ADJ]], [[yy.moreLen]] )");
	    indent_puts_go
		("m4_define( [[YY_RESTORE_YY_MORE_OFFSET]], [[]] )");
	}
    }

    else {
	/* TODO
	    indent_puts_go
		("#define yymore() yymore_used_but_not_detected");
	*/
	indent_puts_go ("m4_define( [[YY_MORE_ADJ]], [[0]] )");
	indent_puts_go ("");
	indent_puts_go ("m4_define( [[YY_RESTORE_YY_MORE_OFFSET]], [[]] )");
    }

    if (!C_plus_plus) {
	if (yytext_is_array) {
	    outn ("#ifndef YYLMAX");
	    outn ("#define YYLMAX 8192");
	    outn ("#endif\n");
	    if (!reentrant){
                outn ("char yytext[YYLMAX]");
                outn ("char *yytext_ptr");
            }
	}

	else {
	    // if(! reentrant)
	    //    outn ("var yytext []byte");
	}
    }

    out (&action_array[defs1_offset]);

    line_directive_out (stdout, 0);

    skelout ();		/* %% [5.0] - break point in skel */

    outn ("// nothing here, all moved to skeleton");

    skelout ();		/* %% [6.0] - break point in skel */

    indent_puts_go ("m4_define( [[YY_RULE_SETUP]], [[");
    indent_up_go ();
    if (bol_needed) {
	indent_puts_go ("if yy.Leng > 0 {");
	indent_up_go ();
	indent_puts_go ("if yy.Text[yy.Leng-1] == '\\n' {");
	indent_up_go ();
	indent_puts_go ("yy.atBol = 1");
	indent_down_go ();
	indent_puts_go ("} else {");
	indent_up_go ();
	indent_puts_go ("yy.atBol = 0");
	indent_down_go ();
	indent_puts_go ("}");
	indent_down_go ();
	indent_puts_go ("}");
    }
    indent_puts_go ("if yy.UserAction != nil {");
    indent_up_go ();
    indent_puts_go ("yy.UserAction(yy)");
    indent_down_go ();
    indent_puts_go ("}");
    indent_puts_go ("yytext = yy.Text");
    indent_puts_go ("yyleng = yy.Leng");
    if (do_yylineno) {
	indent_puts_go ("yylineno = yy.Lineno");
    }
    indent_down_go ();
    indent_puts_go ("]])");

    skelout ();		/* %% [7.0] - break point in skel */

    /* Copy prolog to output file. */
    out (&action_array[prolog_offset]);

    line_directive_out (stdout, 0);

    skelout ();		/* %% [8.0] - break point in skel */

    set_indent_go (2);

    if (yymore_used) {
	indent_puts_go ("yy.moreLen = 0");
	indent_puts_go ("if yy.moreFlag {");
	indent_up_go ();
	indent_puts_go
	    ("yy.moreLen = yy.cBufP - yy.textPtr");
	indent_puts_go ("yy.moreFlag = false");
	indent_down_go ();
	indent_puts_go ("}");
    }

    skelout ();		/* %% [9.0] - break point in skel */

    gen_start_state_go ();

    /* Note, don't use any indentation. */
    outn ("yyMatch:");
    gen_next_match_go ();

    skelout ();		/* %% [10.0] - break point in skel */
    set_indent_go (2);
    gen_find_action_go ();

    skelout ();		/* %% [11.0] - break point in skel */
    outn ("m4_ifdef( [[M4_YY_USE_LINENO]],[[");
    indent_puts_go
	("if yy.act != yyEndOfBuffer && yy_rule_can_match_eol[yy.act] != 0 {");
    indent_up_go ();
    do_indent_go ();
    out_str ("for yyl := %s; yyl < yy.Leng; yyl++ {\n",
	     yymore_used ? (yytext_is_array ? "yy_prev_more_offset" :
			    "yy.moreLen") : "0");
    indent_up_go ();
    indent_puts_go ("if yy.Text[yyl] == '\\n' {");
    indent_up_go ();
    indent_puts_go ("M4_YY_INCR_LINENO");
    indent_down_go ();
    indent_puts_go ("}");
    indent_down_go ();
    indent_puts_go ("}");
    indent_down_go ();
    indent_puts_go ("}");
    outn ("]])");

    skelout ();		/* %% [12.0] - break point in skel */
    if (ddebug) {
	indent_puts_go ("if yyFlexDebug {");
	indent_up_go ();

	indent_puts_go ("if yy.act == 0 {");
	indent_up_go ();
	indent_puts_go ("fmt.Fprintln(os.Stderr, \"--scanner backing up\")");
	indent_down_go ();

	do_indent_go ();
	out_dec ("} else if yy.act < %d {\n", num_rules);
	indent_up_go ();

	indent_puts_go ("fmt.Fprintf(os.Stderr, \"--accepting rule at line %d (%q)\\n\",");
	indent_puts_go ("         yyRuleLinenum[yy.act], yy.Text)");

	indent_down_go ();

	do_indent_go ();
	out_dec ("} else if yy.act == %d {\n", num_rules);
	indent_up_go ();

	indent_puts_go ("fmt.Fprintf(os.Stderr, \"--accepting default rule (%q)\\n\", yy.Text)");

	indent_down_go ();

	do_indent_go ();
	out_dec ("} else if yy.act == %d {\n", num_rules + 1);
	indent_up_go ();

	indent_puts_go ("fmt.Fprintln(os.Stderr, \"--(end of buffer or a NUL)\")");

	indent_down_go ();

	do_indent_go ();
	outn ("} else {");
	indent_up_go ();

	indent_puts_go
	    ("fmt.Fprintf(os.Stderr, \"--EOF (start condition %d)\\n\", YY_START)");

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
	    out_str ("case YY_STATE_EOF(%s):\n", scname[i]);
	    indent_up_go ();
	    indent_puts_go ("yyterminate(YYnull)");
	    indent_down_go ();
	}

    /* Generate code for handling NUL's, if needed. */

    /* First, deal with backing up and setting up yy_cp if the scanner
     * finds that it should JAM on the NUL.
     */
    skelout ();		/* %% [14.0] - break point in skel */
    set_indent_go (4);

    if (fullspd || fulltbl)
	indent_puts_go ("yy.cp = yy.cBufP");

    else {			/* compressed table */
	if (!reject && !interactive) {
	    /* Do the guaranteed-needed backing up to figure
	     * out the match.
	     */

	    indent_puts_go ("yy.cp = yy.lastAcceptingCpos");
	    indent_puts_go ("yy.currentState = yy.lastAcceptingState");

	}

	else {
	    /* Still need to initialize yy_cp, though
	     * yy.currentState was set up by
	     * yy_get_previous_state().
	     */
	    indent_puts_go ("yy.cp = yy.cBufP");
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
	    ("// TODO: yy_CURRENT_BUFFER_LVALUE->yy_at_bol = (c == '\\n')");
	if (do_yylineno) {
	    indent_puts_go
		("// TODO: if ( yy_CURRENT_BUFFER_LVALUE->yy_at_bol )");
	    indent_up_go ();
	    indent_puts_go ("// TODO: M4_YY_INCR_LINENO()");
	    indent_down_go ();
	}
    }

    else if (do_yylineno) {
	indent_puts_go ("if  c == '\\n' {");
	indent_up_go ();
	indent_puts_go ("M4_YY_INCR_LINENO()");
	indent_down_go ();
	indent_puts_go ("}");
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
