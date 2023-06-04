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

ALL_DIRECTIVES:
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
    -return.
    -cgi.
    -upload.

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
    -cgi.
    -upload.

ALLOW_DIRECTIVE_IN_LOCATION:
    -root.
    -index.
    -status_page.
    -allow_methods.
    -client_max_body_size.
    -autoindex.
    -return.
    -cgi.
    -upload.
