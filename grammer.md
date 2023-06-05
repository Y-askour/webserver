<DEFAULT_FILE> ::= <SERVER>
            | <SERVER>...

<SERVER> ::= <OPEN_BRACES> | <CLOSE_BRACES>
        | <OPEN_BRACES> | <DIRACTIVE>... | <CLOSE_BRACES>
        | <OPEN_BRACES> | <DIRACTIVE>... | <LOCATION>... | <CLOSE_BRACES>

<LOCATION> ::= <WORD> | <OPEN_BRACES> | <CLOSE_BRACES>
            | <WORD> | <OPEN_BRACES> | <DIRECTIVE>... | <CLOSE_BRACES>

<DIRECTIVE> ::= <WORD>... | <SEMICOLON>

<SEMICOLON> ::= ;

<OPEN_BRACES> ::= {

<CLOSE_BRACES> ::= }

0 = server only
1 = location only
2 = both
ALL_DIRECTIVES:
0    -listen.
0    -host.
0    -mime_types.
0    -server_name.
0    -status_page.
1    -return.
2    -root.
2    -index.
2    -allow_methods.
2    -client_max_body_size.
2    -autoindex.
2    -cgi_info.
2    -upload.

ALLOW_DIRECTIVE_IN_SERVER:
    -listen.
    -host.
    -root.
    -index.
    -mime_types.
    -server_name.
    -status_page.
    -allow_methods.
    -client_max_body_size.
    -autoindex.
    -cgi_info.
    -upload.

ALLOW_DIRECTIVE_IN_LOCATION:
    -root.
    -index.
    -status_page.
    -allow_methods.
    -client_max_body_size.
    -autoindex.
    -return.
    -cgi_info.
    -upload.
