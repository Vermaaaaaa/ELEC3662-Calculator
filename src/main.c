#include "utility.h"
#include "clock.h"
#include "lcd.h"
#include "keypad.h"
#include "token.h"
#include <stdio.h>
#include <math.h>

#define EPSILON 0.00001

static int verify_tokens(token_t *tokens,
                         unsigned char token_count,
                         token_t *expected_tokens,
                         unsigned char expected_count)
{
    // 1) Check the total count of tokens
    if (token_count != expected_count) {
        return 1; // mismatch in number of tokens
    }

    // 2) Compare each token
    for (unsigned char i = 0; i < token_count; i++) {
        // Compare token_type
        if (tokens[i].token_type != expected_tokens[i].token_type) {
            return 2; // type mismatch
        }

        // If numeric token, compare the values
        if (tokens[i].token_type == TOKEN_NUM) {
            double diff = tokens[i].data.value - expected_tokens[i].data.value;
            if (fabs(diff) > EPSILON) {
                return 3; // numeric value mismatch
            }
        }
        // If operator or function, compare the op codes
        else if (tokens[i].token_type == TOKEN_OP ||
                 tokens[i].token_type == TOKEN_FUNC)
        {
            if (tokens[i].data.op != expected_tokens[i].data.op) {
                return 4; // operator/function mismatch
            }
        }
    }

    // All tokens match
    return 0;
}

// Structure to hold one test case definition
typedef struct {
    const char *test_name;           // Name or description of the test
    keypad_value input[MAX_INPUT_BUFFER];   // Input sequence
    token_t      expected_tokens[MAX_TOKENS]; // Expected token sequence
    unsigned char expected_count;    // How many tokens we expect
    int          expected_return;    // The return code we expect from tokenise()
} token_test_case_t;

// This function will run all of your test cases in sequence.
// Returns 0 if all pass, or (test_index + 1) if a test fails.
int run_all_token_tests(void)
{
    // Define all your test cases here:
    // For demonstration, we include 10 typical tests.
    // Adjust keypad_value enumerations and data as necessary.
    token_test_case_t testcases[] = {

        // Test 1: Simple addition: 1 + 2
        {
            "1 + 2",
            { ONE, PLUS, TWO, NULL_VALUE },
            {
                { TOKEN_NUM,  { .value = 1.0      }, 0 },
                { TOKEN_OP,   { .op    = PLUS     }, 1 },
                { TOKEN_NUM,  { .value = 2.0      }, 2 }
            },
            3,              // expected_count
            SUCCESS         // expected_return
        },

        // Test 2: Unary minus in front: - 3
        {
            "- 3",
            { MINUS, THREE, NULL_VALUE },
            {
                { TOKEN_FUNC, { .op = UNARY_MINUS }, 0 },
                { TOKEN_NUM,  { .value = 3.0       }, 1 }
            },
            2,
            SUCCESS
        },

        // Test 3: Decimal number: 3.14
        {
            "3.14",
            { THREE, DECIMAL, ONE, FOUR, NULL_VALUE },
            {
                { TOKEN_NUM, { .value = 3.14 }, 0 }
            },
            1,
            SUCCESS
        },

        // Test 4: Consecutive minus => plus: 3 - - 3
        {
            "3 - - 3",
            { THREE, MINUS, MINUS, THREE, NULL_VALUE },
            {
                { TOKEN_NUM, { .value = 3.0   }, 0 },
                { TOKEN_OP,  { .op    = PLUS  }, 1 },
                { TOKEN_NUM, { .value = 3.0   }, 2 }
            },
            3,
            SUCCESS
        },

        // Test 5: Power operator: 2 ^ 3
        {
            "2 ^ 3",
            { TWO, POWER, THREE, NULL_VALUE },
            {
                { TOKEN_NUM, { .value = 2.0    }, 0 },
                { TOKEN_OP,  { .op    = POWER  }, 1 },
                { TOKEN_NUM, { .value = 3.0    }, 2 }
            },
            3,
            SUCCESS
        },

        // Test 6: Sine function: sin 30
        {
            "sin 30",
            { SIN, THREE, ZERO, NULL_VALUE },
            {
                { TOKEN_FUNC, { .op    = SIN   }, 0 },
                { TOKEN_NUM,  { .value = 30.0  }, 1 }
            },
            2,
            SUCCESS
        },

        // Test 7: arcsin 0.5 => arcsin, DECIMAL, 5
        {
            "arcsin 0.5",
            { ARCSIN, DECIMAL, FIVE, NULL_VALUE },
            {
                { TOKEN_FUNC, { .op    = ARCSIN }, 0 },
                { TOKEN_NUM,  { .value = 0.5    }, 1 }
            },
            2,
            SUCCESS
        },

        // Test 8: Unary minus in front of a function: - sin 30
        {
            "- sin 30",
            { MINUS, SIN, THREE, ZERO, NULL_VALUE },
            {
                { TOKEN_FUNC, { .op = UNARY_MINUS }, 0 },
                { TOKEN_FUNC, { .op = SIN         }, 1 },
                { TOKEN_NUM,  { .value = 30.0     }, 2 }
            },
            3,
            SUCCESS
        },

        // Test 9: Mixed operators with unary minus: 2 + - 3
        {
            "2 + - 3",
            { TWO, PLUS, MINUS, THREE, NULL_VALUE },
            {
                { TOKEN_NUM,  { .value = 2.0       }, 0 },
                { TOKEN_OP,   { .op    = PLUS      }, 1 },
                { TOKEN_FUNC, { .op    = UNARY_MINUS }, 2 },
                { TOKEN_NUM,  { .value = 3.0       }, 3 }
            },
            4,
            SUCCESS
        },

        // Test 10: Multiple decimals => 2 . 3 . 4 => Should cause DECIMAL_ERROR
        {
            "2.3.4 => error",
            { TWO, DECIMAL, THREE, DECIMAL, FOUR, NULL_VALUE },
            {
                // We expect no tokens if an error is returned
                // Or we can leave this empty
            },
            0,
            DECIMAL_ERROR
        }
    };

    // Number of tests we have
    int num_tests = sizeof(testcases) / sizeof(testcases[0]);

    // Temporary storage for each test
    token_t tokens[MAX_TOKENS];
    unsigned char token_count = 0;

    // Run through each test
    for (int i = 0; i < num_tests; i++) {
        // Clear out tokens array
        for (int k = 0; k < MAX_TOKENS; k++) {
            tokens[k].token_type = NULL_TOKEN;
            tokens[k].data.value = 0.0;
            tokens[k].token_index = 0;
        }
        token_count = 0;

        // 1) Call tokenise on the input
        int ret = tokenise(testcases[i].input, tokens, &token_count);

        // 2) Check the return code
        if (ret != testcases[i].expected_return) {
            // Return code mismatch
            // Return test index + 1 to indicate which test failed
            // (because 0 means "all passed")
            return i + 1;
        }

        // If we expected SUCCESS, we also verify the token contents
        if (ret == SUCCESS) {
            int verify_result = verify_tokens(tokens,
                                              token_count,
                                              testcases[i].expected_tokens,
                                              testcases[i].expected_count);
            if (verify_result != 0) {
                // Token mismatch
                return i + 1; 
            }
        }

        // Otherwise, if ret was DECIMAL_ERROR (or some other error code),
        // we don’t check tokens further.
    }

    // If we get here, all tests passed
    return 0;
}








int main() {
   
	int pll = PLL_init();
	int tick = SysTick_init();
	int gpio = GPIO_init();	
	int lcd = lcd_init();
	int keypad = keypad_init();
	

	/*
	keypad_value input_buffer[MAX_INPUT_BUFFER];
  token_t tokens[MAX_TOKENS];
  unsigned char token_index = 0;
	unsigned char input_buffer_index = 0;
	*/
	
	/*
	token_t tokens[MAX_TOKENS];
	unsigned char token_count = 0;
keypad_value input[] = {TWO, MINUS, MINUS, THREE, NULL_VALUE};


token_t expected[] = {
	{TOKEN_NUM, {.value = 2}, 0},
	{TOKEN_OP, {.op = PLUS}, 1},
	{TOKEN_NUM, {.value = 3}, 2},
};

unsigned char expected_count = 3;

	
	int temp = tokenise(input, tokens, &token_count);
	

	
	int result = verify_tokens(tokens, token_count, expected, expected_count);


 // Display token count
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "Count: %d", token_count);
    lcd_print(buffer);
    delay_millisecond(255);

    // Display each token
    for (unsigned char i = 0; i < token_count; i++) {
        if (tokens[i].token_type == TOKEN_NUM) {
            snprintf(buffer, sizeof(buffer), "NUM: %.2f", tokens[i].data.value);
        } else if (tokens[i].token_type == TOKEN_OP) {
            snprintf(buffer, sizeof(buffer), "OP: %d", tokens[i].data.op);
        } else if (tokens[i].token_type == TOKEN_FUNC) {
            snprintf(buffer, sizeof(buffer), "FUNC: %d", tokens[i].data.op);
        }
        lcd_print(buffer);
        delay_millisecond(255); // Delay to view each token
    }
	
		
	if(result == 0){
		lcd_print("SUCCESS");
	}
	else if(result == 1){lcd_print("FAIL: COUNT MISMATCH");}
	else if(result == 2){lcd_print("FAIL: TYPE MISMATCH");}
	else if(result == 3){lcd_print("FAIL: NUMERIC MISMATCH");}		
	else{
		lcd_print("FAIL: OP/FUNC MISMATCH");
	}
	*/
	
	int failed_test = run_all_token_tests();
if (failed_test == 0) {
    // All passed
    lcd_print("ALL TESTS PASSED");
} else {
    // Some test failed
    lcd_print("TEST FAILED");
}

	
	

	while (1) {
				
		/*
       keypad_value key = get_key(); 
       if(key == NULL_VALUE){ continue; }
			 
			 if(key == EQUALS){
				 input_buffer[input_buffer_index] = NULL_VALUE;
				 int tokenise_result = tokenise(input_buffer, tokens, &token_index);
				 
			 }
			 
			 input_buffer[input_buffer_index] = key;
			 input_buffer_index++;
			
			 const char *str = get_key_string(key);
				
				lcd_print(str);
			 
			*/
		
		
				
				
		
		
	}
		
		
	
		return 0;
}
