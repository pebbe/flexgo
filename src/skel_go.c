/* File created from flex_go.skl via mkskel_go.sh */

#include "flexdef.h"

const char *skel1_go[] = {
  "%# -*-go-*-",
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
  "m4_define( [[M4_YY_INCR_LINENO]], [[",
  "    yy.Lineno++",
  "]])",
  "m4_define( [[INITIAL]], [[yyInitial]] )",
  "m4_define( [[BEGIN]], [[yy.start = 1 + 2*]] )",
  "m4_define( [[ECHO]], [[yy.Echo()]] )",
  "m4_define( [[REJECT]], [[yy.Reject()]] )",
  "m4_define( [[yyterminate]], [[return $1]] )",
  "m4_define( [[yyless]], [[yy.Less($1)]] )",
  "m4_define( [[YY_START]], [[((yy.start - 1) / 2)]] )",
  "m4_define( [[YTSTATE]], [[((yy.start - 1) / 2)]] )",
  "m4_define( [[YY_STATE_EOF]], [[(yyEndOfBuffer + $1 + 1)]] )",
  "",
  "type Scanner struct {",
  "	In   io.Reader",
  "	Out  io.Writer",
  "	Text []byte",
  "	Leng int",
  "	m4_ifdef( [[M4_YY_USE_LINENO]], [[Lineno int]])",
  "",
  "	Filename      string",
  "	UserData      interface{}",
  "	UserAction    func(*Scanner)",
  "	Wrap          func(*Scanner) bool",
  "	IsInteractive func(io.Reader) bool",
  "",
  "	lastAcceptingState   int",
  "	lastAcceptingCpos    int",
  "	debug                bool",
  "	start                int",
  "	stateBuf             []int",
  "	statePtr             int",
  "	currentState         int",
  "	fullState            int",
  "	fullMatch            int",
  "	fullLp               int",
  "	lp                   int",
  "	cp, bp               int",
  "	act                  int",
  "	holdChar             byte",
  "	cBufP                int",
  "	didBufferSwitchOnEof bool",
  "	textPtr              int",
  "	nChars               int",
  "	init                 bool",
  "	moreFlag             bool",
  "    moreLen              int",
  "",
  "	// buffer",
  "	inputFile    io.Reader",
  "	chBuf        []byte // input buffer",
  "	bufPos       int    // current position in input buffer",
  "	bufSize      int",
  "	bufNChars    int",
  "	Interactive  bool",
  "	atBol        int // 0 (falase) or 1 (true)",
  "	fillBuffer   bool",
  "	bufferStatus int",
  "}",
  "",
  "func NewScanner() *Scanner {",
  "	yy := Scanner{",
  "		m4_ifdef( [[M4_YY_USE_LINENO]], [[Lineno: 1,]])",
  "		Wrap:          func(yyy *Scanner) bool { return true },",
  "		IsInteractive: func(file io.Reader) bool { return yyInteractiveDefault },",
  "		bufSize:       yyBufSize,",
  "		chBuf:         make([]byte, yyBufSize+2),",
  "		start:         1,",
  "		stateBuf:      make([]int, 0),",
  "		atBol:         1,",
  "		debug:         yyFlexDebug,",
  "		fillBuffer:    true,",
  "	}",
  "	return &yy",
  "}",
  "",
  "func (yy *Scanner) NewFile() {",
  "	yy.Restart(yy.In)",
  "}",
  "",
  "const yyEndOfBufferChar = 0",
  "",
  "const yyBufSize = 32768",
  "",
  "const (",
  "	eobActEndOfFile    = 0",
  "	eobActContinueScan = 1",
  "	eobActLastMatch    = 2",
  ")",
  "",
  "m4_ifdef( [[M4_YY_USE_LINENO]],",
  "[[",
  "func (yy *Scanner) lessLineno(n int) {",
  "	for yyl := n; yyl < yy.Leng; yyl++ {",
  "		if yy.Text[yyl] == '\\n' {",
  "			yy.Lineno--",
  "		}",
  "	}",
  "}",
  "",
  "func (yy *Scanner) linenoRewindTo(dst int) {",
  "	for p := yy.cp - 1; p >= dst; p-- {",
  "		if yy.chBuf[p] == '\\n' {",
  "			yy.Lineno--",
  "		}",
  "	}",
  "}",
  "]])",
  "",
  "const (",
  "	yyBufferNew        = 0",
  "	yyBufferNormal     = 1",
  "	yyBufferEofPending = 2",
  ")",
  "",
  "// [1.0] the user's section 1 definitions and yytext/yyin/yyout/yy_state_type/yylineno etc. def's & init go here",
  "%% [1.0] the user's section 1 definitions and yytext/yyin/yyout/yy_state_type/yylineno etc. def's & init go here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "// [1.5] DFA------------------------------------------------------------",
  "%% [1.5] DFA",
  "// SKEL ----------------------------------------------------------------",
  "",
  "func (yy *Scanner) doBeforeAction() {",
  "	yy.textPtr = yy.bp",
  "",
  "// [2.0] code to fiddle yytext and yyleng for yy''more() goes here -------",
  "%% [2.0] code to fiddle yytext and yyleng for yy''more() goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	yy.holdChar = yy.chBuf[yy.cp]",
  "	yy.chBuf[yy.cp] = 0",
  "",
  "// [3.0] code to copy yytext_ptr to yytext[] goes here, if %array ------",
  "%% [3.0] code to copy yytext_ptr to yytext[] goes here, if %array",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	yy.cBufP = yy.cp",
  "	yy.Text = yy.chBuf[yy.textPtr:yy.cp]",
  "}",
  "",
  "// [4.0] data tables for the DFA go here -------------------------------",
  "%% [4.0] data tables for the DFA go here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "M4_YY_SC_DEFS",
  "",
  "const yyReadBufSize = 16384",
  "",
  "func (yy *Scanner) Echo() {",
  "	yy.Out.Write(yy.Text)",
  "}",
  "",
  "func (yy *Scanner) input(offset, maxRead int) int {",
  "",
  "// [5.0] fread()/read() definition of yy_INPUT goes here ---------------",
  "%% [5.0] fread()/read() definition of yy_INPUT goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	if yy.Interactive {",
  "		b := make([]byte, 1)",
  "		var n int",
  "		for n = 0; n < maxRead; n++ {",
  "			nn, err := yy.inputFile.Read(b)",
  "			if nn < 1 {",
  "				if err != nil && err != io.EOF {",
  "					log.Panicln(\"input in flex scanner failed:\", err)",
  "				}",
  "				break",
  "			}",
  "			yy.chBuf[offset+n] = b[0]",
  "			if b[0] == '\\n' {",
  "				n++",
  "				break",
  "			}",
  "		}",
  "		return n",
  "	}",
  "",
  "	b := make([]byte, maxRead)",
  "	n, err := yy.inputFile.Read(b)",
  "	if err != nil && err != io.EOF {",
  "		log.Panicln(\"input in flex scanner failed:\", err)",
  "	}",
  "	copy(yy.chBuf[offset:], b[:n])",
  "	//for i := 0; i < n; i++ {",
  "	//	yy.chBuf[offset+i] = b[i]",
  "	//}",
  "	return n",
  "}",
  "",
  "%if-tables-serialization structures and prototypes",
  "// TODO: flex.skl line 1182-1216",
  "",
  "/* end tables serialization structures and prototypes */",
  "%endif",
  "",
  "// TODO: The bison pure parser, flex.skl line 1231-1250",
  "",
  "/* [6.0] YY_RULE_SETUP definition goes here --------------------------*/",
  "%% [6.0] YY_RULE_SETUP definition goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "func (yy *Scanner) Less(n int) {",
  "	// Undo effects of setting up yytext.",
  "	yyless_macro_arg := n",
  "	m4_ifdef( [[M4_YY_USE_LINENO]], [[yy.lessLineno(yyless_macro_arg)]])",
  "	yy.chBuf[yy.cBufP] = yy.holdChar",
  "	YY_RESTORE_YY_MORE_OFFSET",
  "	yy.cp = yy.bp + yyless_macro_arg - YY_MORE_ADJ",
  "	yy.cBufP = yy.cp",
  "	yy.doBeforeAction()",
  "}",
  "",
  "// The main scanner function which does all the work.",
  "func (yy *Scanner) Lex() YYtype {",
  "",
  "	if !yy.init {",
  "		yy.init = true",
  "		if yy.In == nil {",
  "			yy.In = os.Stdin",
  "		}",
  "		if yy.Out == nil {",
  "			yy.Out = os.Stdout",
  "		}",
  "		yy.initBuffer(yy.In)",
  "		yy.loadBufferState()",
  "	}",
  "",
  "	var yytext []byte",
  "	var yyleng int",
  "	m4_ifdef( [[M4_YY_USE_LINENO]], [[var yylineno int]])",
  "	yyout := yy.Out",
  "	if false {",
  "		fmt.Print(yytext, yyleng, yyout m4_ifdef( [[M4_YY_USE_LINENO]], [[, yylineno]]))",
  "	}",
  "",
  "// [7.0] user's declarations go here -----------------------------------",
  "%% [7.0] user''s declarations go here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	for { // loops until end-of-file is reached",
  "",
  "// [8.0] yy''more-related code goes here -------------------------------",
  "%% [8.0] yy''more-related code goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "		yy.cp = yy.cBufP",
  "",
  "		/* Support of yytext. */",
  "		yy.chBuf[yy.cp] = yy.holdChar",
  "",
  "		// yy.bp points to the position in yy_ch_buf of the start of",
  "		// the current run.",
  "		yy.bp = yy.cp",
  "",
  "// [9.0] code to set up and find next match goes here ------------------",
  "%% [9.0] code to set up and find next match goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	yyFindAction:",
  "",
  "// [10.0] code to find the action number goes here ---------------------",
  "%% [10.0] code to find the action number goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "		yy.doBeforeAction()",
  "",
  "// [11.0] code for yylineno update goes here ---------------------------",
  "%% [11.0] code for yylineno update goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	doAction: // This label is used only to access EOF actions.",
  "",
  "// [12.0] debug code goes here -----------------------------------------",
  "%% [12.0] debug code goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "		switch yy.act { // beginning of action switch",
  "",
  "// [13.0] actions go here ----------------------------------------------",
  "%% [13.0] actions go here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "		case yyEndOfBuffer:",
  "			/* Amount of text matched not including the EOB char. */",
  "			yyAmountOfMatchedText := yy.cp - yy.textPtr - 1",
  "",
  "			/* Undo the effects of yy_DO_BEFORE_ACTION. */",
  "			yy.chBuf[yy.cp] = yy.holdChar",
  "			YY_RESTORE_YY_MORE_OFFSET",
  "",
  "			if yy.bufferStatus == yyBufferNew {",
  "				/* We're scanning a new file or input source.  It's",
  "				 * possible that this happened because the user",
  "				 * just pointed yyin at a new source and called",
  "				 * yylex().  If so, then we have to assure",
  "				 * consistency between yy_CURRENT_BUFFER and our",
  "				 * globals.  Here is the right place to do so, because",
  "				 * this is the first action (other than possibly a",
  "				 * back-up) that will match for the new input source.",
  "				 */",
  "				yy.nChars = yy.bufNChars",
  "				yy.inputFile = yy.In",
  "				yy.bufferStatus = yyBufferNormal",
  "			}",
  "",
  "			/* Note that here we test for yy_c_buf_p \"<=\" to the position",
  "			 * of the first EOB in the buffer, since yy_c_buf_p will",
  "			 * already have been incremented past the NUL character",
  "			 * (since all states make transitions on EOB to the",
  "			 * end-of-buffer state).  Contrast this with the test",
  "			 * in input().",
  "			 */",
  "			if yy.cBufP <= yy.nChars {",
  "				/* This was really a NUL. */",
  "				var yyNextState int",
  "",
  "				yy.cBufP = yy.textPtr + yyAmountOfMatchedText",
  "",
  "				yy.currentState = yy.getPreviousState()",
  "",
  "				/* Okay, we're now positioned to make the NUL",
  "				 * transition.  We couldn't have",
  "				 * yy_get_previous_state() go ahead and do it",
  "				 * for us because it doesn't know how to deal",
  "				 * with the possibility of jamming (and we don't",
  "				 * want to build jamming into it because then it",
  "				 * will run more slowly).",
  "				 */",
  "",
  "				yyNextState = yy.tryNulTrans(yy.currentState)",
  "",
  "				yy.bp = yy.textPtr + YY_MORE_ADJ",
  "",
  "				if yyNextState != 0 {",
  "					/* Consume the NUL. */",
  "					yy.cBufP++",
  "					yy.cp = yy.cBufP",
  "					yy.currentState = yyNextState",
  "					goto yyMatch",
  "				} else {",
  "",
  "// [14.0] code to do back-up for compressed tables and set up yy_cp goes here",
  "%% [14.0] code to do back-up for compressed tables and set up yy_cp goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "					goto yyFindAction",
  "				}",
  "",
  "			} else {",
  "",
  "				switch yy.getNextBuffer() {",
  "				case eobActEndOfFile:",
  "					yy.didBufferSwitchOnEof = false",
  "",
  "					if yy.Wrap(yy) {",
  "						// Note: because we've taken care in",
  "						// yy_get_next_buffer() to have set up",
  "						// yytext, we can now set up",
  "						// yy.cBufP so that if some total",
  "						// hoser (like flex itself) wants to",
  "						// call the scanner after we return the",
  "						// yy_NULL, it'll still work - another",
  "						// yy_NULL will get returned.",
  "						yy.cBufP = yy.textPtr + YY_MORE_ADJ",
  "",
  "						yy.act = YY_STATE_EOF(YY_START)",
  "						goto doAction",
  "					} else {",
  "						if !yy.didBufferSwitchOnEof {",
  "							yy.NewFile()",
  "						}",
  "					}",
  "				case eobActContinueScan:",
  "					yy.cBufP = yy.textPtr + yyAmountOfMatchedText",
  "",
  "					yy.currentState = yy.getPreviousState()",
  "",
  "					yy.cp = yy.cBufP",
  "					yy.bp = yy.textPtr + YY_MORE_ADJ",
  "					goto yyMatch",
  "				case eobActLastMatch:",
  "					yy.cBufP = yy.nChars",
  "",
  "					yy.currentState = yy.getPreviousState()",
  "",
  "					yy.cp = yy.cBufP",
  "					yy.bp = yy.textPtr + YY_MORE_ADJ",
  "					goto yyFindAction",
  "				}",
  "			}",
  "",
  "		default:",
  "			log.Panicln(\"fatal flex scanner internal error--no action found:\", yy.act)",
  "		} // end of action switch",
  "	} // end of scanning one token",
  "	return nil",
  "} // end of yylex",
  "",
  "/* yy_get_next_buffer - try to read in a new buffer",
  " *",
  " * Returns a code representing an action:",
  " *	EOB_ACT_LAST_MATCH -",
  " *	EOB_ACT_CONTINUE_SCAN - continue scanning from current position",
  " *	EOB_ACT_END_OF_FILE - end of file",
  " */",
  "func (yy *Scanner) getNextBuffer() int {",
  "",
  "	var numberToMove int",
  "	var retval int",
  "",
  "	if yy.cBufP > yy.nChars+1 {",
  "		log.Panic(\"fatal flex scanner internal error--end of buffer missed\")",
  "	}",
  "",
  "	if !yy.fillBuffer {",
  "		// Don't try to fill the buffer, so this is an EOF.",
  "		if yy.cBufP-yy.textPtr-YY_MORE_ADJ == 1 {",
  "			// We matched a single character, the EOB, so",
  "			// treat this as a final EOF.",
  "			return eobActEndOfFile",
  "		} else {",
  "			// We matched some text prior to the EOB, first",
  "			// process it.",
  "			return eobActLastMatch",
  "		}",
  "	}",
  "",
  "	// Try to read more data.",
  "",
  "	// First move last chars to start of buffer.",
  "	numberToMove = yy.cBufP - yy.textPtr - 1",
  "",
  "	copy(yy.chBuf, yy.chBuf[yy.textPtr:yy.textPtr+numberToMove])",
  "",
  "	if yy.bufferStatus == yyBufferEofPending {",
  "		// don't do the read, it's not guaranteed to return an EOF,",
  "		// just force an EOF",
  "		yy.nChars = 0",
  "		yy.bufNChars = 0",
  "	} else {",
  "		numToRead := yy.bufSize - numberToMove - 1",
  "",
  "		for numToRead <= 0 {",
  "			// Not enough room in the buffer - grow it.",
  "",
  "			yyCBufPOffset := yy.cBufP",
  "",
  "			new_size := yy.bufSize * 2",
  "",
  "			if new_size <= 0 {",
  "				yy.bufSize += yy.bufSize / 8",
  "			} else {",
  "				yy.bufSize *= 2",
  "			}",
  "",
  "			// Include room in for 2 EOB chars.",
  "			bb := make([]byte, yy.bufSize+2-len(yy.chBuf))",
  "			yy.chBuf = append(yy.chBuf, bb...)",
  "",
  "			yy.cBufP = yyCBufPOffset",
  "",
  "			numToRead = yy.bufSize - numberToMove - 1",
  "",
  "		}",
  "",
  "		if numToRead > yyReadBufSize {",
  "			numToRead = yyReadBufSize",
  "		}",
  "",
  "		// Read in more data.",
  "		yy.nChars = yy.input(numberToMove, numToRead)",
  "		yy.bufNChars = yy.nChars",
  "	}",
  "",
  "	if yy.nChars == 0 {",
  "		if numberToMove == YY_MORE_ADJ {",
  "			retval = eobActEndOfFile",
  "			yy.Restart(yy.In)",
  "		} else {",
  "			retval = eobActLastMatch",
  "			yy.bufferStatus = yyBufferEofPending",
  "		}",
  "	} else {",
  "		retval = eobActContinueScan",
  "	}",
  "",
  "	if yy.nChars+numberToMove > yy.bufSize {",
  "		// Extend the array by 50%, plus the number we really need. *",
  "		newSize := yy.nChars + numberToMove + (yy.nChars >> 1)",
  "		if leng := len(yy.chBuf); leng < newSize {",
  "			chBuf := make([]byte, newSize-leng)",
  "			yy.chBuf = append(yy.chBuf, chBuf...)",
  "		}",
  "	}",
  "",
  "	yy.nChars += numberToMove",
  "	yy.bufNChars += numberToMove // TODO: missing in C skel, bug?",
  "	yy.chBuf[yy.nChars] = yyEndOfBufferChar",
  "	yy.chBuf[yy.nChars+1] = yyEndOfBufferChar",
  "",
  "	yy.textPtr = 0",
  "",
  "	return retval",
  "}",
  "",
  "/* yy_get_previous_state - get the state just before the EOB char was reached */",
  "func (yy *Scanner) getPreviousState() int {",
  "",
  "	yyCurrentState := yy.currentState",
  "	yyCp := yy.cp",
  "",
  "// [15.0] code to get the start state into yy_current_state goes here --",
  "%% [15.0] code to get the start state into yy_current_state goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	for yy.cp = yy.textPtr + YY_MORE_ADJ; yy.cp < yy.cBufP; yy.cp++ {",
  "",
  "// [16.0] code to find the next state goes here ------------------------",
  "%% [16.0] code to find the next state goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	}",
  "	newCurrentState := yy.currentState",
  "	yy.currentState = yyCurrentState",
  "	yy.cp = yyCp",
  "	return newCurrentState",
  "}",
  "",
  "",
  "",
  "/* yy_try_NUL_trans - try to make a transition on the NUL character",
  " *",
  " * synopsis",
  " *      next_state = yy_try_NUL_trans( current_state );",
  " */",
  "func (yy *Scanner) tryNulTrans(yyCurrentState int) int {",
  "",
  "	var yyIsJam bool",
  "	oldCurrentState := yy.currentState",
  "	yy.currentState = yyCurrentState",
  "	yyCp := yy.cp",
  "",
  "// [17.0] code to find the next state, and perhaps do backing up, goes here",
  "%% [17.0] code to find the next state, and perhaps do backing up, goes here",
  "// SKEL ----------------------------------------------------------------",
  "",
  "	newCurrentState := yy.currentState",
  "	yy.currentState = oldCurrentState",
  "	yy.cp = yyCp",
  "",
  "	if yyIsJam {",
  "		return 0",
  "	}",
  "	return newCurrentState",
  "}",
  "",
  "m4_ifdef( [[M4_YY_NO_UNPUT]],,[[",
  "func (yy *Scanner) Unput(c byte) {",
  "",
  "	yy_bp := yy.textPtr",
  "    yy_cp := yy.cBufP",
  "",
  "	// undo effects of setting up yytext",
  "	yy.chBuf[yy_cp] = yy.holdChar",
  "",
  "	if yy_cp < 2 {",
  "		// need to shift things up to make room",
  "		// +2 for EOB chars.",
  "",
  "		for yy.nChars + 2 < yy.bufSize {",
  "			yy.chBuf = append(yy.chBuf, yyEndOfBufferChar)",
  "			yy.bufSize++",
  "		}",
  "",
  "		numberToMove := yy.nChars + 2",
  "		moveTo := yy.bufSize + 2 - numberToMove",
  "		copy(yy.chBuf[moveTo:], yy.chBuf[:numberToMove])",
  "",
  "		yy_cp += moveTo",
  "		yy_bp += moveTo",
  "",
  "		yy.nChars += numberToMove",
  "		yy.bufNChars += numberToMove",
  "	}",
  "",
  "	yy_cp--",
  "	yy.chBuf[yy_cp] = c",
  "",
  "",
  "// [18.0] update yylineno here -----------------------------------------",
  "%% [18.0] update yylineno here",
  "// SKEL ----------------------------------------------------------------",
  "m4_ifdef( [[M4_YY_USE_LINENO]],",
  "[[",
  "    if c == '\\n' {",
  "        yy.Lineno--",
  "    }",
  "]])",
  "",
  "	yy.textPtr = yy_bp",
  "	yy.holdChar = yy.chBuf[yy_cp]",
  "	yy.cBufP = yy_cp",
  "}",
  "]])",
  "",
  "",
  "func (yy *Scanner) Input() (byte, error) {",
  "",
  "	yy.chBuf[yy.cBufP] = yy.holdChar",
  "",
  "	if yy.chBuf[yy.cBufP] == yyEndOfBufferChar {",
  "		// yy_c_buf_p now points to the character we want to return.",
  "		// If this occurs *before* the EOB characters, then it's a",
  "		// valid NUL; if not, then we've hit the end of the buffer.",
  "		if yy.cBufP < yy.nChars {",
  "			// This was really a NUL.",
  "			yy.chBuf[yy.cBufP] = 0",
  "		} else {",
  "			// need more input",
  "			offset := yy.cBufP - yy.textPtr",
  "			yy.cBufP++",
  "",
  "			switch yy.getNextBuffer() {",
  "			case eobActLastMatch:",
  "					/* This happens because yy_g_n_b()",
  "					 * sees that we've accumulated a",
  "					 * token and flags that we need to",
  "					 * try matching the token before",
  "					 * proceeding.  But for input(),",
  "					 * there's no matching to consider.",
  "					 * So convert the EOB_ACT_LAST_MATCH",
  "					 * to EOB_ACT_END_OF_FILE.",
  "					 */",
  "",
  "					/* Reset buffer status. */",
  "				yy.Restart(yy.In)",
  "",
  "				fallthrough",
  "",
  "			case eobActEndOfFile:",
  "				if yy.Wrap(yy) {",
  "					return 0, io.EOF",
  "				}",
  "",
  "				if !yy.didBufferSwitchOnEof {",
  "					yy.Restart(yy.In)",
  "				}",
  "",
  "				return yy.Input()",
  "",
  "			case eobActContinueScan:",
  "				yy.cBufP = yy.textPtr + offset",
  "			}",
  "		}",
  "	}",
  "",
  "	c := yy.chBuf[yy.cBufP]",
  "	yy.chBuf[yy.cBufP] = 0	// preserve yytext",
  "	yy.cBufP++",
  "	yy.holdChar = yy.chBuf[yy.cBufP]",
  "",
  "// [19.0] update BOL and yylineno --------------------------------------",
  "%% [19.0] update BOL and yylineno",
  "// SKEL ----------------------------------------------------------------",
  "",
  "return c, nil",
  "}",
  "",
  "/** Immediately switch to a different input stream.",
  " * @param input_file A readable stream.",
  " *",
  " * @note This function does not reset the start condition to @c INITIAL .",
  " */",
  "func (yy *Scanner) Restart(input_file io.Reader) {",
  "	yy.initBuffer(input_file)",
  "	yy.loadBufferState()",
  "}",
  "",
  "func (yy *Scanner) loadBufferState() {",
  "	yy.nChars = yy.bufNChars",
  "	yy.cBufP = yy.bufPos",
  "	yy.textPtr = yy.cBufP",
  "	yy.In = yy.inputFile",
  "	yy.holdChar = yy.chBuf[yy.cBufP]",
  "}",
  "",
  "/* Initializes or reinitializes a buffer.",
  " * This function is sometimes called more than once on the same buffer,",
  " * such as during a yyrestart() or at EOF.",
  " */",
  "func (yy *Scanner) initBuffer(file io.Reader) {",
  "",
  "	yy.flushBuffer()",
  "",
  "	yy.inputFile = file",
  "",
  "	yy.fillBuffer = true",
  "",
  "	yy.Interactive = yy.IsInteractive(file)",
  "",
  "}",
  "",
  "/** Discard all buffered characters. On the next scan, YY_INPUT will be called.",
  " * @param b the buffer state to be flushed, usually @c YY_CURRENT_BUFFER.",
  " *",
  " */",
  "func (yy *Scanner) flushBuffer() {",
  "",
  "	yy.bufNChars = 0",
  "",
  "	/* We always need two end-of-buffer characters.  The first causes",
  "	 * a transition to the end-of-buffer state.  The second causes",
  "	 * a jam in that state.",
  "	 */",
  "	yy.chBuf[0] = yyEndOfBufferChar",
  "	yy.chBuf[1] = yyEndOfBufferChar",
  "",
  "	yy.bufPos = 0",
  "",
  "	yy.atBol = 1",
  "	yy.bufferStatus = yyBufferNew",
  "",
  "	yy.loadBufferState()",
  "}",
  "",
  "func yyIfElse(b bool, i1, i2 int) int {",
  "	if b {",
  "		return i1",
  "	}",
  "	return i2",
  "}",
  "",
  "func YYmain(filenames ...string) (interface{}, error) {",
  "",
  "	var errval error",
  "",
  "	yy := NewScanner()",
  "",
  "	if len(filenames) > 0 {",
  "		yy.Filename = filenames[0]",
  "		yy.In, errval = os.Open(yy.Filename)",
  "		if errval != nil {",
  "			return nil, errval",
  "		}",
  "		yy.Wrap = func(yyy *Scanner) bool {",
  "			yyy.In.(*os.File).Close()",
  "			filenames = filenames[1:]",
  "			if len(filenames) == 0 {",
  "				return true",
  "			}",
  "			yyy.Filename = filenames[0]",
  "			yyy.In, errval = os.Open(yyy.Filename)",
  "			if errval != nil {",
  "				return true",
  "			}",
  "			return false",
  "		}",
  "	}",
  "",
  "	return yy.Lex(), errval",
  "",
  "}",
  "",
  "// END OF SKELL --------------------------------------------------------",
  0
};
const char **skel_go = skel1_go;
