/* File created from flex_go.skl via mkskel_go.sh */

#include "flexdef.h"

const char *skel1_go[] = {
  "// START OF SKELL ------------------------------------------------------",
  "// A lexical scanner generated by flexgo",
  "",
  "%#  Macros for preproc stage.",
  "",
  "",
  "%# Macros for runtime processing stage.",
  "m4_changecom",
  "m4_changequote",
  "m4_changequote([[, ]])",
  "",
  "// [1.0] the user's section 1 definitions and yytext/yyin/yyout/yy_state_type/yylineno etc. def's & init go here",
  "%% [1.0] the user's section 1 definitions and yytext/yyin/yyout/yy_state_type/yylineno etc. def's & init go here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "const (",
  "	yy_END_OF_BUFFER_CHAR = 0",
  "	INITIAL               = 0",
  "",
  "	eob_ACT_END_OF_FILE   = 0",
  "	eob_ACT_CONTINUE_SCAN = 1",
  "	eob_ACT_LAST_MATCH    = 2",
  "",
  "	yy_BUFFER_NEW         = 0",
  "	yy_BUFFER_NORMAL      = 1",
  "	yy_BUFFER_EOF_PENDING = 2",
  "",
  "	yy_BUF_SIZE      = 32768",
  "	yy_READ_BUF_SIZE = 16384",
  ")",
  "",
  "var (",
  "	YYin                     io.Reader",
  "	YYout                    io.Writer",
  "	YYleng                   int",
  "	YYwrap                   = func() bool { return true }",
  "	YY_USER_INIT             = func() { }",
  "	YY_USER_ACTION           = func() { }",
  "	yy_buffer_stack          []*yy_buffer_state",
  "	yy_buffer_stack_top      = 0",
  "	yy_c_buf_p               int",
  "	yy_init                  = false",
  "	yy_running               bool",
  "	yy_rejected		 bool",
  "	yy_n_chars               int",
  "	yy_start                 = 0",
  "	yytext_ptr               int",
  "	yy_hold_char             byte",
  ")",
  "",
  "type yy_buffer_state struct {",
  "	yy_input_file io.Reader",
  "",
  "	yy_ch_buf  []byte // input buffer",
  "	yy_buf_pos int    // current position in input buffer",
  "",
  "	/* Size of input buffer in bytes, not including room for EOB",
  "	 * characters.",
  "	 */",
  "	yy_buf_size int",
  "",
  "	/* Number of characters read into yy_ch_buf, not including EOB",
  "	 * characters.",
  "	 */",
  "	yy_n_chars int",
  "",
  "	/* Whether we \"own\" the buffer - i.e., we know we created it,",
  "	 * and can realloc() it to grow it, and should free() it to",
  "	 * delete it.",
  "	 */",
  "	yy_is_our_buffer bool",
  "",
  "	/* Whether this is an \"interactive\" input source; if so, and",
  "	 * if we're using stdio for input, then we want to use getc()",
  "	 * instead of fread(), to make sure we stop fetching input after",
  "	 * each newline.",
  "	 */",
  "	yy_is_interactive bool",
  "",
  "	/* Whether we're considered to be at the beginning of a line.",
  "	 * If so, '^' rules will be active on the next match, otherwise",
  "	 * not.",
  "	 */",
  "	yy_at_bol int",
  "",
  "	yy_bs_lineno int /**< The line count. */",
  "	yy_bs_column int /**< The column count. */",
  "",
  "	/* Whether to try to fill the input buffer when we reach the",
  "	 * end of it.",
  "	 */",
  "	yy_fill_buffer bool",
  "",
  "	yy_buffer_status int",
  "}",
  "",
  "func yy_CURRENT_BUFFER() *yy_buffer_state {",
  "	if yy_buffer_stack == nil {",
  "		return nil",
  "	}",
  "	return yy_buffer_stack[yy_buffer_stack_top]",
  "}",
  "",
  "func yy_CURRENT_BUFFER_LVALUE() **yy_buffer_state {",
  "	if yy_buffer_stack == nil {",
  "		return nil",
  "	}",
  "	return &yy_buffer_stack[yy_buffer_stack_top]",
  "}",
  "",
  "func yyensure_buffer_stack() {",
  "	if yy_buffer_stack == nil {",
  "		yy_buffer_stack = make([]*yy_buffer_state, 1)",
  "	}",
  "}",
  "",
  "func yy_init_buffer(b *yy_buffer_state, file io.Reader) {",
  "",
  "	yy_flush_buffer(b)",
  "",
  "	b.yy_input_file = file",
  "",
  "	b.yy_fill_buffer = true",
  "",
  "	/* If b is the current buffer, then yy_init_buffer was _probably_",
  "	 * called from yyrestart() or through yy_get_next_buffer.",
  "	 * In that case, we don't want to reset the lineno or column.",
  "	 */",
  "	if b != yy_CURRENT_BUFFER() {",
  "		b.yy_bs_lineno = 1",
  "		b.yy_bs_column = 0",
  "	}",
  "",
  "	b.yy_is_interactive = IsInteractive(file)",
  "}",
  "",
  "func yy_flush_buffer(b *yy_buffer_state) {",
  "	if b == nil {",
  "		return",
  "	}",
  "",
  "	b.yy_n_chars = 0",
  "",
  "	/* We always need two end-of-buffer characters.  The first causes",
  "	 * a transition to the end-of-buffer state.  The second causes",
  "	 * a jam in that state.",
  "	 */",
  "	b.yy_ch_buf[0] = yy_END_OF_BUFFER_CHAR",
  "	b.yy_ch_buf[1] = yy_END_OF_BUFFER_CHAR",
  "",
  "	b.yy_buf_pos = 0",
  "",
  "	b.yy_at_bol = 1",
  "	b.yy_buffer_status = yy_BUFFER_NEW",
  "",
  "	if b == yy_CURRENT_BUFFER() {",
  "		yy_load_buffer_state()",
  "	}",
  "}",
  "",
  "func yy_create_buffer(file io.Reader, size int) *yy_buffer_state {",
  "	b := yy_buffer_state{",
  "		yy_buf_size:      size,",
  "		yy_ch_buf:        make([]byte, size+2),",
  "		yy_is_our_buffer: true,",
  "	}",
  "	yy_init_buffer(&b, file)",
  "	return &b",
  "}",
  "",
  "/* yy_get_next_buffer - try to read in a new buffer",
  " *",
  " * Returns a code representing an action:",
  " *	EOB_ACT_LAST_MATCH -",
  " *	EOB_ACT_CONTINUE_SCAN - continue scanning from current position",
  " *	EOB_ACT_END_OF_FILE - end of file",
  " */",
  "func yy_get_next_buffer() int {",
  "",
  "	// just a shorter name for the current buffer",
  "	buffer := yy_CURRENT_BUFFER()",
  "",
  "	var number_to_move int",
  "	var ret_val int",
  "",
  "	if yy_c_buf_p > yy_n_chars+1 {",
  "		log.Panic(\"fatal flex scanner internal error--end of buffer missed\")",
  "	}",
  "",
  "	if !buffer.yy_fill_buffer {",
  "		// Don't try to fill the buffer, so this is an EOF.",
  "		if yy_c_buf_p-yytext_ptr-yy_MORE_ADJ() == 1 {",
  "			// We matched a single character, the EOB, so",
  "			// treat this as a final EOF.",
  "			return eob_ACT_END_OF_FILE",
  "		} else {",
  "			// We matched some text prior to the EOB, first",
  "			// process it.",
  "			return eob_ACT_LAST_MATCH",
  "		}",
  "	}",
  "",
  "	// Try to read more data.",
  "",
  "	// First move last chars to start of buffer.",
  "	number_to_move = yy_c_buf_p - yytext_ptr - 1",
  "",
  "	copy(buffer.yy_ch_buf, buffer.yy_ch_buf[yytext_ptr:yytext_ptr+number_to_move])",
  "	//for i := 0; i < number_to_move; i++ {",
  "	//	buffer.yy_ch_buf[i] = buffer.yy_ch_buf[i+yytext_ptr]",
  "	//}",
  "",
  "	if buffer.yy_buffer_status == yy_BUFFER_EOF_PENDING {",
  "		// don't do the read, it's not guaranteed to return an EOF,",
  "		// just force an EOF",
  "		yy_n_chars = 0",
  "		buffer.yy_n_chars = 0",
  "	} else {",
  "		num_to_read := buffer.yy_buf_size - number_to_move - 1",
  "",
  "		for num_to_read <= 0 {",
  "			// Not enough room in the buffer - grow it.",
  "",
  "			yy_c_buf_p_offset := yy_c_buf_p",
  "",
  "			if buffer.yy_is_our_buffer {",
  "				new_size := buffer.yy_buf_size * 2",
  "",
  "				if new_size <= 0 {",
  "					buffer.yy_buf_size += buffer.yy_buf_size / 8",
  "				} else {",
  "					buffer.yy_buf_size *= 2",
  "				}",
  "",
  "				// Include room in for 2 EOB chars.",
  "				bb := make([]byte, buffer.yy_buf_size+2-len(buffer.yy_ch_buf))",
  "				buffer.yy_ch_buf = append(buffer.yy_ch_buf, bb...)",
  "			} else {",
  "				// Can't grow it, we don't own it.",
  "				buffer.yy_ch_buf = nil",
  "			}",
  "",
  "			if buffer.yy_ch_buf == nil {",
  "				log.Panicln(",
  "					\"fatal error - scanner input buffer overflow\")",
  "			}",
  "",
  "			yy_c_buf_p = yy_c_buf_p_offset",
  "",
  "			num_to_read = buffer.yy_buf_size - number_to_move - 1",
  "",
  "		}",
  "",
  "		if num_to_read > yy_READ_BUF_SIZE {",
  "			num_to_read = yy_READ_BUF_SIZE",
  "		}",
  "",
  "		// Read in more data.",
  "		yy_n_chars = yy_INPUT(number_to_move, num_to_read)",
  "		buffer.yy_n_chars = yy_n_chars",
  "	}",
  "",
  "	if yy_n_chars == 0 {",
  "		if number_to_move == yy_MORE_ADJ() {",
  "			ret_val = eob_ACT_END_OF_FILE",
  "			YYrestart(YYin)",
  "		} else {",
  "			ret_val = eob_ACT_LAST_MATCH",
  "			buffer.yy_buffer_status = yy_BUFFER_EOF_PENDING",
  "		}",
  "	} else {",
  "		ret_val = eob_ACT_CONTINUE_SCAN",
  "	}",
  "",
  "	if yy_n_chars+number_to_move > buffer.yy_buf_size {",
  "		// Extend the array by 50%, plus the number we really need. *",
  "		new_size := yy_n_chars + number_to_move + (yy_n_chars >> 1)",
  "		if leng := len(buffer.yy_ch_buf); leng < new_size {",
  "			ch_buf := make([]byte, new_size-leng)",
  "			buffer.yy_ch_buf = append(buffer.yy_ch_buf, ch_buf...)",
  "		}",
  "	}",
  "",
  "	yy_n_chars += number_to_move",
  "	buffer.yy_n_chars += number_to_move // TODO: missing in C skel, bug?",
  "	buffer.yy_ch_buf[yy_n_chars] = yy_END_OF_BUFFER_CHAR",
  "	buffer.yy_ch_buf[yy_n_chars+1] = yy_END_OF_BUFFER_CHAR",
  "",
  "	yytext_ptr = 0",
  "",
  "	return ret_val",
  "}",
  "",
  "func yy_load_buffer_state() {",
  "	buffer := yy_CURRENT_BUFFER()",
  "	yy_n_chars = buffer.yy_n_chars",
  "	yy_c_buf_p = buffer.yy_buf_pos",
  "	yytext_ptr = yy_c_buf_p",
  "	YYin = buffer.yy_input_file",
  "	yy_hold_char = buffer.yy_ch_buf[yy_c_buf_p]",
  "}",
  "",
  "// [1.5] DFA------------------------------------------------------------",
  "%% [1.5] DFA",
  "// SKEL ----------------------------------------------------------------",
  "",
  "func yy_DO_BEFORE_ACTION(yy_cp, yy_bp int) {",
  "	buffer := yy_CURRENT_BUFFER()",
  "	yytext_ptr = yy_bp",
  "// [2.0] code to fiddle yytext and yyleng for yymore() goes here -------",
  "%% [2.0] code to fiddle yytext and yyleng for yymore() goes here",
  "// SKEL ----------------------------------------------------------------",
  "	YYleng = yy_cp - yy_bp",
  "	yy_hold_char = buffer.yy_ch_buf[yy_cp]",
  "	buffer.yy_ch_buf[yy_cp] = 0",
  "// [3.0] code to copy yytext_ptr to yytext[] goes here, if %array ------",
  "%% [3.0] code to copy yytext_ptr to yytext[] goes here, if %array",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	YYtext = buffer.yy_ch_buf[yy_bp:yy_cp]",
  "",
  "	yy_c_buf_p = yy_cp",
  "}",
  "",
  "// [4.0] data tables for the DFA go here -------------------------------",
  "%% [4.0] data tables for the DFA go here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "func yy_INPUT(offset, max_read int) int {",
  "	buffer := yy_CURRENT_BUFFER()",
  "// [5.0] fread()/read() definition of YY_INPUT goes here ---------------",
  "%% [5.0] fread()/read() definition of YY_INPUT goes here",
  "// SKEL ----------------------------------------------------------------",
  "	if buffer.yy_is_interactive {",
  "		b := make([]byte, 1)",
  "		var n int",
  "		for n = 0; n < max_read; n++ {",
  "			nn, err := buffer.yy_input_file.Read(b)",
  "			if nn < 1 {",
  "				if err != nil && err != io.EOF {",
  "					log.Panicln(\"input in flex scanner failed:\", err)",
  "				}",
  "				break",
  "			}",
  "			buffer.yy_ch_buf[offset+n] = b[0]",
  "			if b[0] == '\\n' {",
  "				n++",
  "				break",
  "			}",
  "		}",
  "		return n",
  "	}",
  "",
  "	b := make([]byte, max_read)",
  "	n, err := buffer.yy_input_file.Read(b)",
  "	if err != nil && err != io.EOF {",
  "		log.Panicln(\"input in flex scanner failed:\", err)",
  "	}",
  "	for i := 0; i < n; i++ {",
  "		buffer.yy_ch_buf[offset+i] = b[i]",
  "	}",
  "	return n",
  "}",
  "",
  "// [6.0] YY_RULE_SETUP definition goes here ----------------------------",
  "%% [6.0] YY_RULE_SETUP definition goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "var yy_cp int",
  "",
  "func YYlex() {",
  "	var yy_current_state int",
  "	var yy_bp int",
  "	var yy_act int",
  "",
  "	if !yy_init {",
  "		yy_init = true",
  "",
  "		YY_USER_INIT()",
  "",
  "		if yy_start == 0 {",
  "			yy_start = 1 // first start state",
  "		}",
  "",
  "		if YYin == nil {",
  "			YYin = os.Stdin",
  "		}",
  "",
  "		if YYout == nil {",
  "			YYout = os.Stdout",
  "		}",
  "",
  "		if yy_CURRENT_BUFFER() == nil {",
  "			yyensure_buffer_stack ()",
  "			*(yy_CURRENT_BUFFER_LVALUE()) =",
  "				yy_create_buffer(YYin, yy_BUF_SIZE)",
  "		}",
  "",
  "		yy_load_buffer_state()",
  "	}",
  "",
  "	{",
  "// [7.0] user's declarations go here -----------------------------------",
  "%% [7.0] user's declarations go here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "		yy_running = true",
  "		for yy_running { // loops until end-of-file is reached",
  "",
  "			buffer := yy_CURRENT_BUFFER()",
  "",
  "// [8.0] yymore()-related code goes here -------------------------------",
  "%% [8.0] yymore()-related code goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "			yy_cp = yy_c_buf_p",
  "",
  "			/* Support of yytext. */",
  "			buffer.yy_ch_buf[yy_cp] = yy_hold_char",
  "",
  "			// yy_bp points to the position in yy_ch_buf of the start of",
  "			// the current run.",
  "			yy_bp = yy_cp",
  "",
  "// [9.0] code to set up and find next match goes here ------------------",
  "%% [9.0] code to set up and find next match goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "yy_find_action:",
  "			buffer = yy_CURRENT_BUFFER()",
  "",
  "// [10.0] code to find the action number goes here ---------------------",
  "%% [10.0] code to find the action number goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "			yy_DO_BEFORE_ACTION(yy_cp, yy_bp)",
  "",
  "// [11.0] code for yylineno update goes here ---------------------------",
  "%% [11.0] code for yylineno update goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "do_action: // This label is used only to access EOF actions.",
  "			buffer = yy_CURRENT_BUFFER()",
  "",
  "// [12.0] debug code goes here -----------------------------------------",
  "%% [12.0] debug code goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "			yy_rejected = false",
  "			switch yy_act { // beginning of action switch",
  "// [13.0] actions go here ----------------------------------------------",
  "%% [13.0] actions go here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "			case yy_END_OF_BUFFER:",
  "				/* Amount of text matched not including the EOB char. */",
  "				yy_amount_of_matched_text := yy_cp - yytext_ptr - 1",
  "",
  "				/* Undo the effects of YY_DO_BEFORE_ACTION. */",
  "				buffer.yy_ch_buf[yy_cp] = yy_hold_char",
  "				yy_RESTORE_YY_MORE_OFFSET()",
  "",
  "				if buffer.yy_buffer_status == yy_BUFFER_NEW {",
  "					/* We're scanning a new file or input source.  It's",
  "					 * possible that this happened because the user",
  "					 * just pointed yyin at a new source and called",
  "					 * yylex().  If so, then we have to assure",
  "					 * consistency between YY_CURRENT_BUFFER and our",
  "					 * globals.  Here is the right place to do so, because",
  "					 * this is the first action (other than possibly a",
  "					 * back-up) that will match for the new input source.",
  "					 */",
  "					yy_n_chars = buffer.yy_n_chars",
  "					buffer.yy_input_file = YYin",
  "					buffer.yy_buffer_status = yy_BUFFER_NORMAL",
  "				}",
  "",
  "				/* Note that here we test for yy_c_buf_p \"<=\" to the position",
  "				 * of the first EOB in the buffer, since yy_c_buf_p will",
  "				 * already have been incremented past the NUL character",
  "				 * (since all states make transitions on EOB to the",
  "				 * end-of-buffer state).  Contrast this with the test",
  "				 * in input().",
  "				 */",
  "				if yy_c_buf_p <= yy_n_chars {",
  "					/* This was really a NUL. */",
  "					var yy_next_state int",
  "",
  "					yy_c_buf_p = yytext_ptr + yy_amount_of_matched_text",
  "",
  "					yy_current_state = yy_get_previous_state()",
  "",
  "					/* Okay, we're now positioned to make the NUL",
  "					 * transition.  We couldn't have",
  "					 * yy_get_previous_state() go ahead and do it",
  "					 * for us because it doesn't know how to deal",
  "					 * with the possibility of jamming (and we don't",
  "					 * want to build jamming into it because then it",
  "					 * will run more slowly).",
  "					 */",
  "",
  "					yy_next_state = yy_try_NUL_trans(yy_current_state)",
  "",
  "					yy_bp = yytext_ptr + yy_MORE_ADJ()",
  "",
  "					if yy_next_state != 0 {",
  "						/* Consume the NUL. */",
  "						yy_c_buf_p++",
  "						yy_cp = yy_c_buf_p",
  "						yy_current_state = yy_next_state",
  "						goto yy_match",
  "					} else {",
  "",
  "// [14.0] code to do back-up for compressed tables and set up yy_cp goes here",
  "%% [14.0] code to do back-up for compressed tables and set up yy_cp goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "						goto yy_find_action",
  "					}",
  "",
  "				} else {",
  "",
  "					switch yy_get_next_buffer() {",
  "					case eob_ACT_END_OF_FILE:",
  "						yy_did_buffer_switch_on_eof := false",
  "",
  "						if YYwrap() {",
  "							// Note: because we've taken care in",
  "							// yy_get_next_buffer() to have set up",
  "							// yytext, we can now set up",
  "							// yy_c_buf_p so that if some total",
  "							// hoser (like flex itself) wants to",
  "							// call the scanner after we return the",
  "							// YY_NULL, it'll still work - another",
  "							// YY_NULL will get returned.",
  "							yy_c_buf_p = yytext_ptr + yy_MORE_ADJ()",
  "",
  "							yy_act = yy_STATE_EOF(yy_START())",
  "							goto do_action",
  "						} else {",
  "							if !yy_did_buffer_switch_on_eof {",
  "								yy_NEW_FILE()",
  "							}",
  "						}",
  "					case eob_ACT_CONTINUE_SCAN:",
  "						yy_c_buf_p = yytext_ptr + yy_amount_of_matched_text",
  "",
  "						yy_current_state = yy_get_previous_state()",
  "",
  "						yy_cp = yy_c_buf_p",
  "						yy_bp = yytext_ptr + yy_MORE_ADJ()",
  "						goto yy_match",
  "					case eob_ACT_LAST_MATCH:",
  "						yy_c_buf_p = yy_n_chars",
  "",
  "						yy_current_state = yy_get_previous_state()",
  "",
  "						yy_cp = yy_c_buf_p",
  "						yy_bp = yytext_ptr + yy_MORE_ADJ()",
  "						goto yy_find_action",
  "					}",
  "				}",
  "",
  "			default:",
  "				log.Panicln(\"fatal flex scanner internal error--no action found:\", yy_act)",
  "			} /* end of action switch */",
  "			if yy_rejected {",
  "				goto find_rule",
  "			}",
  "		} /* end of scanning one token */",
  "	} /* end of user's declarations */",
  "",
  "} /* end of yylex */",
  "",
  "/* yy_get_previous_state - get the state just before the EOB char was reached */",
  "func yy_get_previous_state() int {",
  "",
  "	buffer := yy_CURRENT_BUFFER()",
  "",
  "	var yy_current_state int",
  "	var yy_cp int",
  "",
  "// [15.0] code to get the start state into yy_current_state goes here --",
  "%% [15.0] code to get the start state into yy_current_state goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	for yy_cp = yytext_ptr + yy_MORE_ADJ(); yy_cp < yy_c_buf_p; yy_cp++ {",
  "",
  "// [16.0] code to find the next state goes here ------------------------",
  "%% [16.0] code to find the next state goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	}",
  "        return yy_current_state",
  "}",
  "",
  "/* yy_try_NUL_trans - try to make a transition on the NUL character",
  " *",
  " * synopsis",
  " *      next_state = yy_try_NUL_trans( current_state );",
  " */",
  "func yy_try_NUL_trans(yy_current_state int) int {",
  "",
  "	var yy_is_jam bool",
  "",
  "// [17.0] code to find the next state, and perhaps do backing up, goes here",
  "%% [17.0] code to find the next state, and perhaps do backing up, goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	if yy_is_jam {",
  "		return 0",
  "	}",
  "",
  "	return yy_current_state",
  "}",
  "",
  "// [18.0] update yylineno here -----------------------------------------",
  "%% [18.0] update yylineno here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "// [19.0] update BOL and yylineno --------------------------------------",
  "%% [19.0] update BOL and yylineno",
  "// SKEL ----------------------------------------------------------------",
  "",
  "func yy_START() int {",
  "	return (yy_start - 1) / 2",
  "}",
  "",
  "func yy_NEW_FILE() {",
  "	YYrestart(YYin)",
  "}",
  "",
  "func yy_STATE_EOF(state int) int {",
  "	return yy_END_OF_BUFFER + state + 1",
  "}",
  "",
  "func ECHO() {",
  "	YYout.Write(YYtext)",
  "}",
  "",
  "func YYterminate() {",
  "	yy_running = false",
  "}",
  "",
  "/** Immediately switch to a different input stream.",
  " * @param input_file A readable stream.",
  " *",
  " * @note This function does not reset the start condition to @c INITIAL .",
  " */",
  "func YYrestart(input_file io.Reader) {",
  "",
  "	if yy_CURRENT_BUFFER() == nil {",
  "		yyensure_buffer_stack()",
  "		*(yy_CURRENT_BUFFER_LVALUE()) =",
  "			yy_create_buffer(YYin, yy_BUF_SIZE)",
  "	}",
  "",
  "	yy_init_buffer(yy_CURRENT_BUFFER(), input_file)",
  "	yy_load_buffer_state()",
  "}",
  "",
  "func ifelse(b bool, i1, i2 int) int {",
  "	if b {",
  "		return i1",
  "	}",
  "	return i2",
  "}",
  "",
  "func YYmain(filenames ...string) {",
  "",
  "	in := YYin",
  "	wrap := YYwrap",
  "	defer func() {",
  "		YYin = in",
  "		YYwrap = wrap",
  "	}()",
  "",
  "	if len(filenames) > 0 {",
  "		var err error",
  "		YYin, err = os.Open(filenames[0])",
  "		if err != nil {",
  "			log.Fatalln(err)",
  "		}",
  "		YYwrap = func() bool {",
  "			YYin.(*os.File).Close()",
  "			filenames = filenames[1:]",
  "			if len(filenames) == 0 {",
  "				return true",
  "			}",
  "			YYin, err = os.Open(filenames[0])",
  "			if err != nil {",
  "				log.Fatalln(err)",
  "			}",
  "			return false",
  "		}",
  "	}",
  "",
  "	YYlex()",
  "",
  "}",
  "",
  "// END OF SKELL --------------------------------------------------------",
  0
};
const char **skel_go = skel1_go;
