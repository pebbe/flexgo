package main

import (
	"bufio"
	"io"
	"log"
	"os"
)

const (
	YY_END_OF_BUFFER_CHAR = 0
	YY_BUF_SIZE           = 32768
	INITIAL               = 0

	YY_BUFFER_NEW         = 0
	YY_BUFFER_NORMAL      = 1
	YY_BUFFER_EOF_PENDING = 2
)

type yy_buffer_state struct {
	yy_file   io.Reader
	yy_reader *bufio.Reader

	peek   []byte
	eof    bool
	peeked int

	yy_buf_size int

	/* Whether we're considered to be at the beginning of a line.
	 * If so, '^' rules will be active on the next match, otherwise
	 * not.
	 */
	yy_at_bol int

	yy_bs_lineno int /**< The line count. */
	yy_bs_column int /**< The column count. */

	yy_buffer_status int
}

func (b *yy_buffer_state) PeekAt(n int) int {
	if b.peek == nil || n >= len(b.peek) && !b.eof {
		var err error
		b.peek, err = b.yy_reader.Peek(n + 100)
		b.eof = false
		if err != nil {
			if err.Error() == "EOF" {
				b.eof = true
			} else {
				log.Panic(err)
			}
		}
	}
	if len(b.peek) > n {
		return int(b.peek[n])
	}
	return YY_END_OF_BUFFER_CHAR
}

func (b *yy_buffer_state) Max(n int) int {
	if b.peek == nil {
		return 0
	}
	if i := len(b.peek); i < n {
		return i
	}
	return n
}

func (b *yy_buffer_state) Peeked(n int) []byte {
	if i := len(b.peek); i < n {
		n = i
	}
	return b.peek[:n]
}

func (b *yy_buffer_state) Read(n int) []byte {
	if b.peek != nil {
		if len(b.peek) > n {
			b.peek = b.peek[n:]
		} else {
			b.peek = nil
		}
	}
	buf := make([]byte, n)
	nn, err := b.yy_reader.Read(buf)
	if n != nn {
		log.Panic(err)
	}
	return buf
}

var (
	yytext              []byte
	yy_init             = false
	yyin                io.Reader
	yyout               io.Writer
	yy_start            = 0
	yy_buffer_stack     []*yy_buffer_state
	yy_buffer_stack_top = -1
	yywrap              = func() bool { return true }
)

func yy_init_buffer(b *yy_buffer_state, file io.Reader) {
	// TODO: push old value of b.peek
	if yy_buffer_stack_top < 0 || b != yy_buffer_stack[yy_buffer_stack_top] {
		b.yy_at_bol = 1
		b.yy_bs_lineno = 1
		b.yy_bs_column = 0
	}
	if b.yy_file != file {
		b.yy_file = file
		b.yy_reader = bufio.NewReaderSize(file, b.yy_buf_size)
		b.peek = nil
		b.eof = false
	}
	yyin = file
}

func yy_create_buffer(file io.Reader, size int) *yy_buffer_state {
	b := yy_buffer_state{
		yy_file:          file,
		yy_reader:        bufio.NewReaderSize(file, size),
		yy_buf_size:      size,
		yy_buffer_status: YY_BUFFER_NEW,
	}
	yy_init_buffer(&b, file)
	return &b
}

func YY_STATE_EOF(state int) int {
	return YY_END_OF_BUFFER + state + 1
}

/* yy_get_previous_state - get the state just before the EOB char was reached */
func yy_get_previous_state(yy_last_accepting_state int, bytes []byte) (int, int) {

	//yy_last_accepting_cpos := 0

	yy_current_state := yy_start

	i1 := len(bytes)
	ii := i1 + 2
	for yy_cp := 0; yy_cp < ii; yy_cp++ {
		yy_c := 1
		if yy_cp < i1 {
			yy_c = int(yy_ec[bytes[yy_cp]])
		}

		if yy_accept[yy_current_state] != 0 {
			yy_last_accepting_state = yy_current_state
			//yy_last_accepting_cpos = yy_cp
		}

		for yy_chk[yy_base[yy_current_state]+yy_c] != yy_current_state {
			yy_current_state = yy_def[yy_current_state]
			if yy_current_state >= 7 {
				yy_c = int(yy_meta[yy_c])
			}
		}
		yy_current_state = yy_nxt[yy_base[yy_current_state]+yy_c]
	}

	return yy_current_state, yy_last_accepting_state
}

/* yy_try_NUL_trans - try to make a transition on the NUL character
 *
 * synopsis
 *      next_state = yy_try_NUL_trans( current_state );
 */
func yy_try_NUL_trans(yy_current_state, yy_last_accepting_state int) (int, int, bool) {

	ac_cur := false
	yy_c := 1
	if yy_accept[yy_current_state] != 0 {
		yy_last_accepting_state = yy_current_state
		ac_cur = true
	}
	for yy_chk[yy_base[yy_current_state]+yy_c] != yy_current_state {
		yy_current_state = yy_def[yy_current_state]
		if yy_current_state >= 7 {
			yy_c = int(yy_meta[yy_c])
		}
	}
	yy_current_state = yy_nxt[yy_base[yy_current_state]+yy_c]

	if yy_current_state == 6 {
		yy_current_state = 0
	}
	return yy_current_state, yy_last_accepting_state, ac_cur
}

func YY_START() int {
	return (yy_start - 1) / 2
}

func YYSTATE() int {
	return YY_START()
}

func YY_NEW_FILE() {
	yyrestart(yyin)
}

/** Immediately switch to a different input stream.
 * @param input_file A readable stream.
 *
 * @note This function does not reset the start condition to @c INITIAL .
 */
func yyrestart(input_file io.Reader) {

	if yy_buffer_stack == nil {
		yy_buffer_stack = make([]*yy_buffer_state, 1)
		yy_buffer_stack[0] = yy_create_buffer(yyin, YY_BUF_SIZE)
		yy_buffer_stack_top = 0
	}

	yy_init_buffer(yy_buffer_stack[yy_buffer_stack_top], input_file)
}

func yylex() {
	if !yy_init {
		yy_init = true

		// #ifdef YY_USER_INIT
		// YY_USER_INIT
		// #endif

		if yy_start == 0 {
			yy_start = 1 /* first start state */
		}

		if yyin == nil {
			yyin = os.Stdin
		}

		if yyout == nil {
			yyout = os.Stdout
		}

		if yy_buffer_stack == nil {
			yy_buffer_stack = make([]*yy_buffer_state, 1)
			yy_buffer_stack[0] = yy_create_buffer(yyin, YY_BUF_SIZE)
			yy_buffer_stack_top = 0
		}
	}

	{
		//#line 13 "tst.l"

		//#line 696 "tst.c"

		var yy_act, yy_last_accepting_state, yy_last_accepting_cpos int
		for { /* loops until end-of-file is reached */

			var bytes []byte
			var unused int

			buffer := yy_buffer_stack[yy_buffer_stack_top]

			yy_current_state := yy_start

			dont := false
			if dont {
				goto yy_match
			}
			if dont {
				goto yy_find_action
			}
			if dont {
				goto do_action
			}

		yy_match:
			yy_last_accepting_state = 0
			yy_last_accepting_cpos = 0
			for n := 0; true; n++ {
				curbyte := buffer.PeekAt(n)
				yy_c := int(yy_ec[curbyte])
				if yy_accept[yy_current_state] != 0 {
					yy_last_accepting_state = yy_current_state
					yy_last_accepting_cpos = buffer.Max(n)
				}
				for yy_chk[yy_base[yy_current_state]+yy_c] != yy_current_state {
					yy_current_state = yy_def[yy_current_state]
					if yy_current_state >= 7 {
						yy_c = int(yy_meta[yy_c])
					}
				}
				yy_current_state = int(yy_nxt[yy_base[yy_current_state]+yy_c])
				if yy_current_state == 6 {
					bytes = buffer.Peeked(n) // dit moet voor buffer.Read()
					unused = len(bytes) - yy_last_accepting_cpos
					if yy_last_accepting_cpos > 0 {
						yytext = buffer.Read(yy_last_accepting_cpos)
					} else {
						yytext = []byte{}
					}
					yy_current_state = yy_last_accepting_state
					break
				}
			}

		yy_find_action:
			yy_act = yy_accept[yy_current_state]

		do_action: /* This label is used only to access EOF actions. */

			switch yy_act { /* beginning of action switch */
			case 0: /* must back up */
				//fmt.Println("case 0")
				//yy_cp = yy_last_accepting_cpos
				yy_current_state = yy_last_accepting_state
				goto yy_find_action
			case 1:
				//fmt.Println("case 1")
				//YY_RULE_SETUP
				//#line 15 "tst.l"
				yyout.Write([]byte("Go"))
			case 2:
				//fmt.Println("case 2")
				//YY_RULE_SETUP
				//#line 17 "tst.l"
				yyout.Write(yytext)
				//#line 759 "tst.c"
			case YY_STATE_EOF(INITIAL):
				//fmt.Println("case YY_STATE_EOF(INITIAL)")
				return
			case YY_END_OF_BUFFER:
				//fmt.Println("case YY_END_OF_BUFFER\n")

				/* Amount of text matched not including the EOB char. */
				// yy_amount_of_matched_text := len(bytes)

				/* Undo the effects of YY_DO_BEFORE_ACTION. */
				//YY_RESTORE_YY_MORE_OFFSET

				if buffer.yy_buffer_status == YY_BUFFER_NEW {
					/* We're scanning a new file or input source.  It's
					 * possible that this happened because the user
					 * just pointed yyin at a new source and called
					 * yylex().  If so, then we have to assure
					 * consistency between YY_CURRENT_BUFFER and our
					 * globals.  Here is the right place to do so, because
					 * this is the first action (other than possibly a
					 * back-up) that will match for the new input source.
					 */
					buffer.yy_buffer_status = YY_BUFFER_NORMAL
					if buffer.yy_file != yyin {
						buffer.yy_file = yyin
						buffer.yy_reader = bufio.NewReaderSize(yyin, buffer.yy_buf_size)
					}
				}

				/* Note that here we test for yy_c_buf_p "<=" to the position
				 * of the first EOB in the buffer, since yy_c_buf_p will
				 * already have been incremented past the NUL character
				 * (since all states make transitions on EOB to the
				 * end-of-buffer state).  Contrast this with the test
				 * in input().
				 */
				if len(bytes) > 0 {
					/* This was really a NUL. */
					var yy_next_state int

					// (yy_c_buf_p) = (yytext_ptr) + yy_amount_of_matched_text;

					yy_current_state, yy_last_accepting_state = yy_get_previous_state(yy_last_accepting_state, bytes)

					/* Okay, we're now positioned to make the NUL
					 * transition.  We couldn't have
					 * yy_get_previous_state() go ahead and do it
					 * for us because it doesn't know how to deal
					 * with the possibility of jamming (and we don't
					 * want to build jamming into it because then it
					 * will run more slowly).
					 */

					var ac bool
					yy_next_state, yy_last_accepting_state, ac = yy_try_NUL_trans(yy_current_state, yy_last_accepting_state)

					// yy_bp = (yytext_ptr) + YY_MORE_ADJ;

					if yy_next_state != 0 {
						/* Consume the NUL. */
						buffer.Read(unused)
						yy_current_state = yy_next_state
						goto yy_match
					} else {
						if ac {
							buffer.Read(unused)
						}
						yy_current_state = yy_last_accepting_state
						goto yy_find_action
					}

				} else {

					yy_did_buffer_switch_on_eof := false

					if yywrap() {
						// Note: because we've taken care in
						// yy_get_next_buffer() to have set up
						// yytext, we can now set up
						// yy_c_buf_p so that if some total
						// hoser (like flex itself) wants to
						// call the scanner after we return the
						// YY_NULL, it'll still work - another
						// YY_NULL will get returned.
						//(yy_c_buf_p) = (yytext_ptr) + YY_MORE_ADJ

						yy_act = YY_STATE_EOF(YY_START())
						goto do_action
					} else {
						if !yy_did_buffer_switch_on_eof {
							YY_NEW_FILE()
						}
					}
				}

			default:
				log.Fatalln("fatal flex scanner internal error--no action found:", yy_act)
			} /* end of action switch */
		} /* end of scanning one token */
	} /* end of user's declarations */

} /* end of yylex */

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

func main() {

	if len(os.Args) > 1 {
		files := os.Args[1:]
		r, err := os.Open(files[0])
		if err != nil {
			log.Fatal(err)
		}
		yyin = r
		yywrap = func() bool {
			r.Close()
			files = files[1:]
			if len(files) == 0 {
				return true
			}
			r, err = os.Open(files[0])
			yyrestart(r)
			if err != nil {
				log.Fatal(err)
			}
			return false
		}
	}

	yylex()
}
