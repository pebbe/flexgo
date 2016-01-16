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
  "	yy_BUFFER_NEW         = 0",
  "	yy_BUFFER_NORMAL      = 1",
  "	yy_BUFFER_EOF_PENDING = 2",
  ")",
  "",
  "var (",
  "        YYin                io.Reader",
  "        YYout               io.Writer",
  "        YYwrap              = func() bool { return true }",
  "        yy_init             = false",
  "        yy_start            = 0",
  "        yy_buffer_stack     []*yy_buffer_state",
  "        yy_buffer_stack_top = -1",
  "	yy_running	    bool",
  ")",
  "",
  "type yy_buffer_state struct {",
  "	yy_file   io.Reader",
  "	yy_reader *bufio.Reader",
  "",
  "	peek   []byte",
  "	backup []byte",
  "	eof    bool",
  "	peeked int",
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
  "	yy_buffer_status int",
  "}",
  "",
  "func (b *yy_buffer_state) Flush() {",
  "     b.yy_reader.ReadLine()",
  "     b.peek = nil",
  "     b.backup = nil",
  "}",
  "",
  "func (b *yy_buffer_state) Push(bytes []byte) {",
  "	if b.backup == nil {",
  "		b.backup = make([]byte, len(bytes))",
  "		copy(b.backup, bytes)",
  "	} else {",
  "		b.backup = append(b.backup, bytes...)",
  "	}",
  "}",
  "",
  "func (b *yy_buffer_state) PeekAt(n int) int {",
  "	var pn, bn int",
  "	if b.peek != nil {",
  "		pn = len(b.peek)",
  "	}",
  "	if b.backup != nil {",
  "		bn = len(b.backup)",
  "	}",
  "	if n >= pn + bn && !b.eof {",
  "		var err error",
  "		b.peek, err = b.yy_reader.Peek(n + 100)",
  "		pn = len(b.peek)",
  "		b.eof = false",
  "		if err != nil {",
  "			if err.Error() == \"EOF\" {",
  "				b.eof = true",
  "			} else {",
  "				log.Panic(err)",
  "			}",
  "		}",
  "	}",
  "	if bn > n {",
  "		return int(b.backup[n])",
  "	}",
  "	n -= bn",
  "	if pn > n {",
  "		return int(b.peek[n])",
  "	}",
  "	return yy_END_OF_BUFFER_CHAR",
  "}",
  "",
  "func (b *yy_buffer_state) Max(n int) int {",
  "	var pn, bn int",
  "	if b.peek != nil {",
  "		pn = len(b.peek)",
  "	}",
  "	if b.backup != nil {",
  "		bn = len(b.backup)",
  "	}",
  "	if i := pn + bn; i < n {",
  "		return i",
  "	}",
  "	return n",
  "}",
  "",
  "func (b *yy_buffer_state) Peeked(n int) []byte {",
  "	var pn, bn int",
  "	if b.peek != nil {",
  "		pn = len(b.peek)",
  "	}",
  "	if b.backup != nil {",
  "		bn = len(b.backup)",
  "	}",
  "	if i := pn + bn; i < n {",
  "		n = i",
  "	}",
  "	if n <= bn {",
  "		return b.backup[:n]",
  "	}",
  "	if bn == 0 && n <= pn {",
  "		return b.peek[:n]",
  "	}",
  "	bb := make([]byte, bn, n)",
  "	copy(bb, b.backup)",
  "	bb = append(bb, b.peek[:n-bn]...)",
  "	return bb",
  "}",
  "",
  "func (b *yy_buffer_state) Read(n int) []byte {",
  "	var pn, bn int",
  "	if b.peek != nil {",
  "		pn = len(b.peek)",
  "	}",
  "	if b.backup != nil {",
  "		bn = len(b.backup)",
  "	}",
  "	bb := make([]byte, 0, n)",
  "	if bn > 0 {",
  "		if n < bn {",
  "			bb = append(bb, b.backup[:n]...)",
  "			b.backup = b.backup[n:]",
  "			bn = len(b.backup)",
  "			n = 0",
  "		} else {",
  "			bb = append(bb, b.backup...)",
  "			b.backup = nil",
  "			n -= bn",
  "			bn = 0",
  "		}",
  "	}",
  "	if n == 0 {",
  "		return bb",
  "	}",
  "",
  "	if b.peek != nil {",
  "		if pn > n {",
  "			b.peek = b.peek[n:]",
  "		} else {",
  "			b.peek = nil",
  "		}",
  "	}",
  "",
  "	buf := make([]byte, n)",
  "	nn, err := b.yy_reader.Read(buf)",
  "	if n != nn {",
  "		log.Panic(err)",
  "	}",
  "	bb = append(bb, buf...)",
  "	return bb",
  "}",
  "",
  "func yy_init_buffer(b *yy_buffer_state, file io.Reader) {",
  "        if yy_buffer_stack_top < 0 || b != yy_buffer_stack[yy_buffer_stack_top] {",
  "                b.yy_at_bol = 1",
  "                b.yy_bs_lineno = 1",
  "                b.yy_bs_column = 0",
  "        }",
  "        if b.yy_file != file {",
  "                b.yy_file = file",
  "                b.yy_reader = bufio.NewReaderSize(file, 100000)",
  "		b.backup = b.peek",
  "                b.peek = nil",
  "                b.eof = false",
  "        }",
  "        YYin = file",
  "}       ",
  "",
  "func yy_create_buffer(file io.Reader) *yy_buffer_state {",
  "	b := yy_buffer_state{",
  "		yy_file:          file,",
  "		yy_reader:        bufio.NewReaderSize(file, 100000),",
  "		yy_buffer_status: yy_BUFFER_NEW,",
  "	}",
  "	yy_init_buffer(&b, file)",
  "	return &b",
  "}",
  "",
  "// [1.5] DFA------------------------------------------------------------",
  "%% [1.5] DFA",
  "// SKEL ----------------------------------------------------------------",
  "",
  "// [2.0] code to fiddle yytext and yyleng for yymore() goes here -------",
  "%% [2.0] code to fiddle yytext and yyleng for yymore() goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "// [3.0] code to copy yytext_ptr to yytext[] goes here, if %array ------",
  "%% [3.0] code to copy yytext_ptr to yytext[] goes here, if %array",
  "// SKEL ----------------------------------------------------------------",
  "",
  "// [4.0] data tables for the DFA go here -------------------------------",
  "%% [4.0] data tables for the DFA go here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "// [5.0] fread()/read() definition of YY_INPUT goes here ---------------",
  "%% [5.0] fread()/read() definition of YY_INPUT goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "// [6.0] YY_RULE_SETUP definition goes here ----------------------------",
  "%% [6.0] YY_RULE_SETUP definition goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "func YYlex() {",
  "	if !yy_init {",
  "		yy_init = true",
  "",
  "		// #ifdef YY_USER_INIT",
  "		// YY_USER_INIT",
  "		// #endif",
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
  "		if yy_buffer_stack == nil {",
  "			yy_buffer_stack = make([]*yy_buffer_state, 1)",
  "			yy_buffer_stack[0] = yy_create_buffer(YYin)",
  "			yy_buffer_stack_top = 0",
  "		}",
  "	}",
  "",
  "	{",
  "// [7.0] user's declarations go here -----------------------------------",
  "%% [7.0] user's declarations go here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "		var yy_act, yy_last_accepting_state, yy_last_accepting_cpos int",
  "		for yy_running = true; yy_running; { // loops until YYterminate()",
  "			var bytes []byte",
  "			var unused int",
  "",
  "			buffer := yy_buffer_stack[yy_buffer_stack_top]",
  "",
  "			var yy_current_state int",
  "			dont := false",
  "			if dont {",
  "				goto yy_match",
  "			}",
  "			if dont {",
  "				goto yy_find_action",
  "			}",
  "			if dont {",
  "				goto do_action",
  "			}",
  "// [8.0] yymore()-related code goes here -------------------------------",
  "%% [8.0] yymore()-related code goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "// [9.0] code to set up and find next match goes here ------------------",
  "%% [9.0] code to set up and find next match goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "yy_find_action:",
  "// [10.0] code to find the action number goes here ---------------------",
  "%% [10.0] code to find the action number goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "// [11.0] code for yylineno update goes here ---------------------------",
  "%% [11.0] code for yylineno update goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "do_action:     // This label is used only to access EOF actions.",
  "",
  "// [12.0] debug code goes here -----------------------------------------",
  "%% [12.0] debug code goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "			switch yy_act { // beginning of action switch",
  "// [13.0] actions go here ----------------------------------------------",
  "%% [13.0] actions go here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "			case yy_END_OF_BUFFER:",
  "				//fmt.Println(\"case yy_END_OF_BUFFER\\n\")",
  "",
  "				// Amount of text matched not including the EOB char.",
  "				// yy_amount_of_matched_text := len(bytes)",
  "",
  "				// Undo the effects of YY_DO_BEFORE_ACTION.",
  "				//YY_RESTORE_YY_MORE_OFFSET",
  "",
  "				if buffer.yy_buffer_status == yy_BUFFER_NEW {",
  "					// We're scanning a new file or input source.  It's",
  "					// possible that this happened because the user",
  "					// just pointed yyin at a new source and called",
  "					// yylex().  If so, then we have to assure",
  "					// consistency between YY_CURRENT_BUFFER and our",
  "					// globals.  Here is the right place to do so, because",
  "					// this is the first action (other than possibly a",
  "					// back-up) that will match for the new input source.",
  "					buffer.yy_buffer_status = yy_BUFFER_NORMAL",
  "					if buffer.yy_file != YYin {",
  "						buffer.yy_file = YYin",
  "						buffer.yy_reader = bufio.NewReaderSize(YYin, 100000)",
  "					}",
  "				}",
  "",
  "				// Note that here we test for yy_c_buf_p \"<=\" to the position",
  "				// of the first EOB in the buffer, since yy_c_buf_p will",
  "				// already have been incremented past the NUL character",
  "				// (since all states make transitions on EOB to the",
  "				// end-of-buffer state).  Contrast this with the test",
  "				// in input().",
  "				if len(bytes) > 0 {",
  "					// This was really a NUL.",
  "					var yy_next_state int",
  "",
  "					// (yy_c_buf_p) = (yytext_ptr) + yy_amount_of_matched_text;",
  "",
  "					yy_current_state, yy_last_accepting_state = yy_get_previous_state(yy_last_accepting_state, bytes)",
  "",
  "					// Okay, we're now positioned to make the NUL",
  "					// transition.  We couldn't have",
  "					// yy_get_previous_state() go ahead and do it",
  "					// for us because it doesn't know how to deal",
  "					// with the possibility of jamming (and we don't",
  "					// want to build jamming into it because then it",
  "					// will run more slowly).",
  "",
  "					var ac bool",
  "					yy_next_state, yy_last_accepting_state, ac = yy_try_NUL_trans(yy_current_state, yy_last_accepting_state)",
  "",
  "					// yy_bp = (yytext_ptr) + YY_MORE_ADJ;",
  "",
  "					if yy_next_state != 0 {",
  "						// Consume the NUL.",
  "						buffer.Read(unused)",
  "						yy_current_state = yy_next_state",
  "						goto yy_match",
  "					} else {",
  "",
  "// [14.0] code to do back-up for compressed tables and set up yy_cp goes here",
  "%% [14.0] code to do back-up for compressed tables and set up yy_cp goes here",
  "// SKEL ----------------------------------------------------------------",
  "						goto yy_find_action",
  "					}",
  "",
  "				} else {",
  "",
  "					yy_did_buffer_switch_on_eof := false",
  "",
  "					if YYwrap() {",
  "						// Note: because we've taken care in",
  "						// yy_get_next_buffer() to have set up",
  "						// yytext, we can now set up",
  "						// yy_c_buf_p so that if some total",
  "						// hoser (like flex itself) wants to",
  "						// call the scanner after we return the",
  "						// YY_NULL, it'll still work - another",
  "						// YY_NULL will get returned.",
  "						//(yy_c_buf_p) = (yytext_ptr) + YY_MORE_ADJ",
  "",
  "						yy_act = yy_STATE_EOF(yy_START())",
  "						goto do_action",
  "					} else {",
  "						if !yy_did_buffer_switch_on_eof {",
  "							yy_NEW_FILE()",
  "						}",
  "					}",
  "				}",
  "",
  "			default:",
  "				log.Fatalln(\"fatal flex scanner internal error--no action found:\", yy_act)",
  "			} // end of action switch",
  "		} // end of scanning one token",
  "	} // end of user's declarations",
  "",
  "} // end of YYlex",
  "",
  "/* yy_get_previous_state - get the state just before the EOB char was reached */",
  "func yy_get_previous_state(yy_last_accepting_state int, bytes []byte) (int, int) {",
  "",
  "	//yy_last_accepting_cpos := 0",
  "",
  "	var yy_current_state int",
  "// [15.0] code to get the start state into yy_current_state goes here --",
  "%% [15.0] code to get the start state into yy_current_state goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	i1 := len(bytes)",
  "	ii := i1 + 2",
  "	for yy_cp := 0; yy_cp < ii; yy_cp++ {",
  "",
  "// [16.0] code to find the next state goes here ------------------------",
  "%% [16.0] code to find the next state goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	}",
  "        return yy_current_state, yy_last_accepting_state",
  "}",
  "",
  "/* yy_try_NUL_trans - try to make a transition on the NUL character",
  " *",
  " * synopsis",
  " *      next_state = yy_try_NUL_trans( current_state );",
  " */",
  "func yy_try_NUL_trans(yy_current_state, yy_last_accepting_state int) (int, int, bool) {",
  "",
  "	ac_cur := false",
  "	yy_is_jam := false",
  "",
  "// [17.0] code to find the next state, and perhaps do backing up, goes here",
  "%% [17.0] code to find the next state, and perhaps do backing up, goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	if yy_is_jam {",
  "		yy_current_state = 0",
  "	}",
  "        return yy_current_state, yy_last_accepting_state, ac_cur",
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
  "     YYout.Write(YYtext)",
  "}",
  "",
  "func YYterminate() {",
  "     yy_running = false",
  "}",
  "",
  "/** Immediately switch to a different input stream.",
  " * @param input_file A readable stream.",
  " *",
  " * @note This function does not reset the start condition to @c INITIAL .",
  " */",
  "func YYrestart(input_file io.Reader) {",
  "",
  "	if yy_buffer_stack == nil {",
  "		yy_buffer_stack = make([]*yy_buffer_state, 1)",
  "		yy_buffer_stack[0] = yy_create_buffer(YYin)",
  "		yy_buffer_stack_top = 0",
  "	}",
  "",
  "	yy_init_buffer(yy_buffer_stack[yy_buffer_stack_top], input_file)",
  "}",
  "",
  "func YYmain(filenames ...string) {",
  "",
  "	if len(filenames) == 0 {",
  "		YYin = os.Stdin",
  " 	} else {",
  "                r, err := os.Open(filenames[0])",
  "                if err != nil {",
  "                        log.Fatal(err)",
  "                }",
  "                YYin = r // YYrestart(r) ???",
  "                YYwrap = func() bool {",
  "                        r.Close()",
  "                        filenames = filenames[1:]",
  "                        if len(filenames) == 0 {",
  "                                return true",
  "                        }",
  "                        r, err = os.Open(filenames[0])",
  "                        YYrestart(r)",
  "                        if err != nil {",
  "                                log.Fatal(err)",
  "                        }",
  "                        return false",
  "                }",
  "        }",
  "",
  "        YYlex()",
  "}",
  "// END OF SKELL --------------------------------------------------------",
  0
};
const char **skel_go = skel1_go;
