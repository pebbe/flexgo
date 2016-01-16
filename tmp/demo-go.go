package main

import (
	//"fmt"
	"bufio"
	"io"
	"log"
	"os"
)

const (
	yy_END_OF_BUFFER_CHAR = 0
	INITIAL               = 0

	eob_ACT_END_OF_FILE   = 0
	eob_ACT_CONTINUE_SCAN = 1
	eob_ACT_LAST_MATCH    = 2

	yy_BUFFER_NEW         = 0
	yy_BUFFER_NORMAL      = 1
	yy_BUFFER_EOF_PENDING = 2
)

var (
	YYleng              int
	yy_init             = false
	YYin                io.Reader
	YYout               io.Writer
	yy_start            = 0
	yy_buffer_stack     []*yy_buffer_state
	yy_buffer_stack_top = -1
	YYwrap              = func() bool { return true }
	yy_c_buf_p          int
	yytext_ptr          int
	yy_running          bool
)

type yy_buffer_state struct {
	yy_file   io.Reader
	yy_reader *bufio.Reader

	peek        []byte
	backup      []byte
	eof         bool
	peeked      int
	nul         int
	is_nul      bool
	YY_MORE_ADJ int

	/* Whether we're considered to be at the beginning of a line.
	 * If so, '^' rules will be active on the next match, otherwise
	 * not.
	 */
	yy_at_bol int

	yy_bs_lineno int /**< The line count. */
	yy_bs_column int /**< The column count. */

	yy_buffer_status int
}

func (b *yy_buffer_state) yy_n_chars() int {
	var n int
	if b.peek != nil {
		n = len(b.peek)
	}
	if b.backup != nil {
		n += len(b.backup)
	}
	return n
}

func (b *yy_buffer_state) isNul(n int) bool {
	return b.is_nul && b.nul == n
}

func (b *yy_buffer_state) clearNul() {
	b.is_nul = false
}

func (b *yy_buffer_state) YY_DO_BEFORE_ACTION(yy_cp, yy_bp int) {
	yytext_ptr = yy_bp
	YYleng = yy_cp - yy_bp
	b.nul = yy_cp
	b.is_nul = true
	yy_c_buf_p = yy_cp
	YYtext = b.Peeked(yy_cp)
	if yy_bp > 0 {
		YYtext = YYtext[yy_bp:]
	}
}

func (b *yy_buffer_state) Push(bytes []byte) {
	if b.backup == nil {
		b.backup = make([]byte, len(bytes))
		copy(b.backup, bytes)
	} else {
		b.backup = append(b.backup, bytes...)
	}
}

func (b *yy_buffer_state) PeekAt(n int) int {
	var pn, bn int
	if b.peek != nil {
		pn = len(b.peek)
	}
	if b.backup != nil {
		bn = len(b.backup)
	}
	if n >= pn+bn && !b.eof {
		var err error
		b.peek, err = b.yy_reader.Peek(n + 100)
		pn = len(b.peek)
		b.eof = false
		if err != nil {
			if err.Error() == "EOF" {
				b.eof = true
			} else {
				log.Panic(err)
			}
		}
	}
	if bn > n {
		return int(b.backup[n])
	}
	n -= bn
	if pn > n {
		return int(b.peek[n])
	}
	return yy_END_OF_BUFFER_CHAR
}

func (b *yy_buffer_state) Peeked(n int) []byte {
	var pn, bn int
	if b.peek != nil {
		pn = len(b.peek)
	}
	if b.backup != nil {
		bn = len(b.backup)
	}
	if i := pn + bn; i < n {
		n = i
	}
	if n <= bn {
		return b.backup[:n]
	}
	if bn == 0 && n <= pn {
		return b.peek[:n]
	}
	bb := make([]byte, bn, n)
	copy(bb, b.backup)
	bb = append(bb, b.peek[:n-bn]...)
	return bb
}

func (b *yy_buffer_state) Advance(n int) {
	//fmt.Printf("\n\nAdvance %d", n)

	if n < 1 {
		return
	}

	if b.backup != nil {
		bn := len(b.backup)
		if n < bn {
			b.backup = b.backup[n:]
			return
		}
		b.backup = nil
		n -= bn
	}

	if n < 1 {
		return
	}

	if b.peek != nil {
		bn := len(b.peek)
		if n < bn {
			b.peek = b.peek[n:]
			return
		}
		b.peek = nil
		n -= bn
	}

	if n < 1 {
		return
	}

	buf := make([]byte, n)
	nn, err := b.yy_reader.Read(buf)
	if n != nn {
		log.Panic(err)
	}
}

func yy_init_buffer(b *yy_buffer_state, file io.Reader) {
	if yy_buffer_stack_top < 0 || b != yy_buffer_stack[yy_buffer_stack_top] {
		b.yy_at_bol = 1
		b.yy_bs_lineno = 1
		b.yy_bs_column = 0
	}
	if b.yy_file != file {
		b.yy_file = file
		b.yy_reader = bufio.NewReaderSize(file, 100000)
		b.backup = b.peek
		b.peek = nil
		b.eof = false
	}
	YYin = file
	yy_c_buf_p = 0
}

func yy_create_buffer(file io.Reader) *yy_buffer_state {
	b := yy_buffer_state{
		yy_file:          file,
		yy_reader:        bufio.NewReaderSize(file, 100000),
		yy_buffer_status: yy_BUFFER_NEW,
	}
	yy_init_buffer(&b, file)
	return &b
}

func YY_STATE_EOF(state int) int {
	return YY_END_OF_BUFFER + state + 1
}

func yy_get_next_buffer(b *yy_buffer_state) int {

	if yy_c_buf_p > b.yy_n_chars()+1 {
		panic("fatal flex scanner internal error--end of buffer missed")
	}

	if yy_c_buf_p-yytext_ptr-b.YY_MORE_ADJ == 1 {
		/* We matched a single character, the EOB, so
		 * treat this as a final EOF.
		 */
		return eob_ACT_END_OF_FILE
	}

	/* We matched some text prior to the EOB, first
	 * process it.
	 */
	return eob_ACT_LAST_MATCH
}

func yy_load_buffer_state() {
	b := yy_buffer_stack[yy_buffer_stack_top]
	yytext_ptr = 0
	yy_c_buf_p = 0
	YYin = b.yy_file
}

const YY_NUM_RULES = 2
const YY_END_OF_BUFFER = 3

/* This struct is not used in this scanner,
   but its presence is necessary. */
//struct yy_trans_info
//	{
//	flex_int32_t yy_verify;
//	flex_int32_t yy_nxt;
//	};
var yy_accept = [7]int{
	0,
	0, 0, 3, 2, 1, 0,
}

var yy_ec = [256]byte{0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1,
}

var yy_meta = [3]byte{
	0,
	1, 1,
}

var yy_base = [7]int{
	0,
	0, 0, 3, 4, 4, 4,
}

var yy_def = [7]int{
	0,
	6, 1, 6, 6, 6, 0,
}

var yy_nxt = [7]int{
	0,
	4, 5, 6, 3, 6, 6,
}

var yy_chk = [7]int{
	0,
	1, 1, 3, 6, 6, 6,
}

var yy_last_accepting_state int
var yy_last_accepting_cpos int

var YYtext []byte

func yylex() {
	var yy_current_state int
	var yy_cp, yy_bp int
	var yy_act int

	if !yy_init {
		yy_init = true

		// #ifdef YY_USER_INIT
		// YY_USER_INIT
		// #endif

		if yy_start == 0 {
			yy_start = 1 /* first start state */
		}

		if YYin == nil {
			YYin = os.Stdin
		}

		if YYout == nil {
			YYout = os.Stdout
		}

		if yy_buffer_stack == nil {
			yy_buffer_stack = make([]*yy_buffer_state, 0, 1)
		}
		if len(yy_buffer_stack) == 0 {
			b := yy_create_buffer(YYin)
			yy_buffer_stack = append(yy_buffer_stack, b)
		}
		yy_buffer_stack_top = len(yy_buffer_stack) - 1

		yy_load_buffer_state()
	}

	{
		//#line 13 "tst.l"

		//#line 696 "tst.c"

		yy_running = true
		for yy_running { // loops until end-of-file is reached

			buffer := yy_buffer_stack[yy_buffer_stack_top]

			// [8.0] yymore()-related code goes here

			buffer.Advance(yy_c_buf_p)
			yy_c_buf_p = 0
			yy_cp = 0

			/* Support of yytext. */
			buffer.clearNul()

			// yy_bp points to the position in yy_ch_buf of the start of
			// the current run.
			yy_bp = yy_cp

			// [9.0] code to set up and find next match goes here
			yy_current_state = yy_start

		yy_match:
			buffer = yy_buffer_stack[yy_buffer_stack_top]
			for {
				yy_c := int(yy_ec[buffer.PeekAt(yy_cp)])
				if yy_accept[yy_current_state] != 0 {
					yy_last_accepting_state = yy_current_state
					yy_last_accepting_cpos = yy_cp
				}
				for int(yy_chk[int(yy_base[yy_current_state])+yy_c]) != yy_current_state {
					yy_current_state = int(yy_def[yy_current_state])
					if yy_current_state >= 7 {
						yy_c = int(yy_meta[yy_c])
					}
				}
				yy_current_state = int(yy_nxt[int(yy_base[yy_current_state])+yy_c])
				yy_cp++
				if yy_current_state == 6 {
					break
				}
			}
			yy_cp = yy_last_accepting_cpos
			yy_current_state = yy_last_accepting_state

		yy_find_action:
			buffer = yy_buffer_stack[yy_buffer_stack_top]
			yy_act = yy_accept[yy_current_state]

			buffer.YY_DO_BEFORE_ACTION(yy_cp, yy_bp)

		do_action: /* This label is used only to access EOF actions. */
			buffer = yy_buffer_stack[yy_buffer_stack_top]

			switch yy_act { /* beginning of action switch */
			case 0: /* must back up */
				//fmt.Printf("\n-- case 0 %q\n", string(YYtext))
				buffer.clearNul()
				yy_cp = yy_last_accepting_cpos
				yy_current_state = yy_last_accepting_state
				goto yy_find_action
			case 1:
				//fmt.Printf("\n-- case 1 %q\n", string(YYtext))
				//YY_RULE_SETUP
				//#line 15 "tst.l"
				YYout.Write([]byte("Go"))
			case 2:
				//fmt.Printf("\n-- case 2 %q\n", string(YYtext))
				//YY_RULE_SETUP
				//#line 17 "tst.l"
				YYout.Write(YYtext)
				//#line 759 "tst.c"
			case YY_STATE_EOF(INITIAL):
				//fmt.Printf("\n-- EOF(INITIAL %q\n", string(YYtext))
				YYterminate()
			case YY_END_OF_BUFFER:
				//fmt.Printf("\n-- END OF BUFFER %q\n", string(YYtext))
				/* Amount of text matched not including the EOB char. */
				yy_amount_of_matched_text := yy_cp - yytext_ptr - 1

				/* Undo the effects of YY_DO_BEFORE_ACTION. */
				buffer.clearNul()
				//TODO: YY_RESTORE_YY_MORE_OFFSET

				if buffer.yy_buffer_status == yy_BUFFER_NEW {
					/* We're scanning a new file or input source.  It's
					 * possible that this happened because the user
					 * just pointed yyin at a new source and called
					 * yylex().  If so, then we have to assure
					 * consistency between YY_CURRENT_BUFFER and our
					 * globals.  Here is the right place to do so, because
					 * this is the first action (other than possibly a
					 * back-up) that will match for the new input source.
					 */
					if buffer.yy_file != YYin {
						buffer.yy_file = YYin
						buffer.yy_reader = bufio.NewReaderSize(YYin, 100000)
						buffer.peek = nil
						buffer.backup = nil
					}
					buffer.yy_buffer_status = yy_BUFFER_NORMAL
				}

				/* Note that here we test for yy_c_buf_p "<=" to the position
				 * of the first EOB in the buffer, since yy_c_buf_p will
				 * already have been incremented past the NUL character
				 * (since all states make transitions on EOB to the
				 * end-of-buffer state).  Contrast this with the test
				 * in input().
				 */
				if yy_c_buf_p <= buffer.yy_n_chars() {
					/* This was really a NUL. */
					var yy_next_state int

					yy_c_buf_p = yytext_ptr + yy_amount_of_matched_text

					yy_current_state = yy_get_previous_state(buffer)

					/* Okay, we're now positioned to make the NUL
					 * transition.  We couldn't have
					 * yy_get_previous_state() go ahead and do it
					 * for us because it doesn't know how to deal
					 * with the possibility of jamming (and we don't
					 * want to build jamming into it because then it
					 * will run more slowly).
					 */

					yy_next_state = yy_try_NUL_trans(buffer, yy_current_state)

					yy_bp = yytext_ptr + buffer.YY_MORE_ADJ

					if yy_next_state != 0 {
						/* Consume the NUL. */
						yy_c_buf_p++
						yy_cp = yy_c_buf_p
						yy_current_state = yy_next_state
						goto yy_match
					} else {
						yy_cp = yy_last_accepting_cpos
						yy_current_state = yy_last_accepting_state
						goto yy_find_action
					}

				} else {

					switch yy_get_next_buffer(buffer) {
					case eob_ACT_END_OF_FILE:
						yy_did_buffer_switch_on_eof := false

						if YYwrap() {
							// Note: because we've taken care in
							// yy_get_next_buffer() to have set up
							// yytext, we can now set up
							// yy_c_buf_p so that if some total
							// hoser (like flex itself) wants to
							// call the scanner after we return the
							// YY_NULL, it'll still work - another
							// YY_NULL will get returned.
							yy_c_buf_p = yytext_ptr + buffer.YY_MORE_ADJ

							yy_act = YY_STATE_EOF(YY_START())
							goto do_action
						} else {
							if !yy_did_buffer_switch_on_eof {
								yy_c_buf_p = 0
								YY_NEW_FILE()
							}
						}
					case eob_ACT_CONTINUE_SCAN:
						// C only
					case eob_ACT_LAST_MATCH:
						yy_c_buf_p = buffer.yy_n_chars()

						yy_current_state = yy_get_previous_state(buffer)

						yy_cp = yy_c_buf_p
						yy_bp = yytext_ptr + buffer.YY_MORE_ADJ
						goto yy_find_action
					}
				}

			default:
				log.Fatalln("fatal flex scanner internal error--no action found:", yy_act)
			} /* end of action switch */
		} /* end of scanning one token */
	} /* end of user's declarations */

} /* end of yylex */

/* yy_get_previous_state - get the state just before the EOB char was reached */
func yy_get_previous_state(buffer *yy_buffer_state) int {

	var yy_current_state int
	var yy_cp int

	yy_current_state = yy_start

	for yy_cp = yytext_ptr + buffer.YY_MORE_ADJ; yy_cp < yy_c_buf_p; yy_cp++ {
		yy_c := ifelse(buffer.isNul(yy_cp), 1, int(yy_ec[buffer.PeekAt(yy_cp)]))
		if yy_accept[yy_current_state] != 0 {
			yy_last_accepting_state = yy_current_state
			yy_last_accepting_cpos = yy_cp
		}

		for yy_chk[yy_base[yy_current_state]+yy_c] != yy_current_state {
			yy_current_state = yy_def[yy_current_state]
			if yy_current_state >= 7 {
				yy_c = int(yy_meta[yy_c])
			}
		}
		yy_current_state = yy_nxt[yy_base[yy_current_state]+yy_c]
	}

	return yy_current_state
}

/* yy_try_NUL_trans - try to make a transition on the NUL character
 *
 * synopsis
 *      next_state = yy_try_NUL_trans( current_state );
 */
func yy_try_NUL_trans(b *yy_buffer_state, yy_current_state int) int {

	var yy_is_jam bool

	yy_cp := yy_c_buf_p
	yy_c := 1
	if yy_accept[yy_current_state] != 0 {
		yy_last_accepting_state = yy_current_state
		yy_last_accepting_cpos = yy_cp
	}
	for yy_chk[yy_base[yy_current_state]+yy_c] != yy_current_state {
		yy_current_state = yy_def[yy_current_state]
		if yy_current_state >= 7 {
			yy_c = int(yy_meta[yy_c])
		}
	}
	yy_current_state = yy_nxt[yy_base[yy_current_state]+yy_c]

	if yy_current_state == 6 {
		yy_is_jam = true
	}

	if yy_is_jam {
		return 0
	}

	return yy_current_state
}

func YY_START() int {
	return (yy_start - 1) / 2
}

func YY_NEW_FILE() {
	YYrestart(YYin)
}

func YYSTATE() int {
	return YY_START()
}

func YYterminate() {
	yy_running = false
}

/** Immediately switch to a different input stream.
 * @param input_file A readable stream.
 *
 * @note This function does not reset the start condition to @c INITIAL .
 */
func YYrestart(input_file io.Reader) {

	if yy_buffer_stack == nil {
		yy_buffer_stack = make([]*yy_buffer_state, 0)
	}
	if len(yy_buffer_stack) == 0 {
		b := yy_create_buffer(input_file)
		yy_buffer_stack = append(yy_buffer_stack, b)
		yy_buffer_stack_top = len(yy_buffer_stack) - 1
	}

	yy_init_buffer(yy_buffer_stack[yy_buffer_stack_top], input_file)
}

func ifelse(b bool, i1, i2 int) int {
	if b {
		return i1
	}
	return i2
}

func main() {

	if len(os.Args) > 1 {
		files := os.Args[1:]
		r, err := os.Open(files[0])
		if err != nil {
			log.Fatal(err)
		}
		YYin = r
		YYwrap = func() bool {
			r.Close()
			files = files[1:]
			if len(files) == 0 {
				return true
			}
			r, err = os.Open(files[0])
			if err != nil {
				log.Fatal(err)
			}
			YYin = r
			return false
		}
	}

	yylex()
	//fmt.Println("Done")
}
