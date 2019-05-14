int cairo_code_pad_get_width() { return 77; }
int cairo_code_pad_get_height() { return 77; }
void cairo_code_pad_render(cairo_t *cr) {
cairo_surface_t *temp_surface;
cairo_t *old_cr;
cairo_pattern_t *pattern;
cairo_matrix_t matrix;

cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
pattern = cairo_pattern_create_rgba(1,0.6,0.333333,1);
cairo_set_source(cr, pattern);
cairo_pattern_destroy(pattern);
cairo_new_path(cr);
cairo_move_to(cr, 73.429688, 75.714844);
cairo_curve_to(cr, 74.027344, 75.714844, 74.621094, 75.460938, 75.046875, 75.039063);
cairo_curve_to(cr, 75.46875, 74.613281, 75.722656, 74.019531, 75.722656, 73.421875);
cairo_line_to(cr, 75.722656, 3.507813);
cairo_curve_to(cr, 75.722656, 2.910156, 75.46875, 2.316406, 75.046875, 1.882813);
cairo_curve_to(cr, 74.621094, 1.457031, 74.027344, 1.214844, 73.429688, 1.214844);
cairo_line_to(cr, 3.503906, 1.214844);
cairo_curve_to(cr, 2.902344, 1.214844, 2.304688, 1.457031, 1.890625, 1.882813);
cairo_curve_to(cr, 1.457031, 2.316406, 1.214844, 2.910156, 1.214844, 3.507813);
cairo_line_to(cr, 1.214844, 73.421875);
cairo_curve_to(cr, 1.214844, 74.019531, 1.457031, 74.613281, 1.890625, 75.039063);
cairo_curve_to(cr, 2.304688, 75.460938, 2.902344, 75.714844, 3.503906, 75.714844);
cairo_close_path(cr);
cairo_move_to(cr, 73.429688, 75.714844);
cairo_set_tolerance(cr, 0.1);
cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
cairo_set_fill_rule(cr, CAIRO_FILL_RULE_WINDING);
cairo_fill_preserve(cr);
 /********************/
cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
cairo_set_line_width(cr, 2.4262);
cairo_set_miter_limit(cr, 4);
cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT);
cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
pattern = cairo_pattern_create_rgba(0,0,0,1);
cairo_set_source(cr, pattern);
cairo_pattern_destroy(pattern);
cairo_new_path(cr);
cairo_move_to(cr, 73.429688, 75.714844);
cairo_curve_to(cr, 74.027344, 75.714844, 74.621094, 75.460938, 75.046875, 75.039063);
cairo_curve_to(cr, 75.46875, 74.613281, 75.722656, 74.019531, 75.722656, 73.421875);
cairo_line_to(cr, 75.722656, 3.507813);
cairo_curve_to(cr, 75.722656, 2.910156, 75.46875, 2.316406, 75.046875, 1.882813);
cairo_curve_to(cr, 74.621094, 1.457031, 74.027344, 1.214844, 73.429688, 1.214844);
cairo_line_to(cr, 3.503906, 1.214844);
cairo_curve_to(cr, 2.902344, 1.214844, 2.304688, 1.457031, 1.890625, 1.882813);
cairo_curve_to(cr, 1.457031, 2.316406, 1.214844, 2.910156, 1.214844, 3.507813);
cairo_line_to(cr, 1.214844, 73.421875);
cairo_curve_to(cr, 1.214844, 74.019531, 1.457031, 74.613281, 1.890625, 75.039063);
cairo_curve_to(cr, 2.304688, 75.460938, 2.902344, 75.714844, 3.503906, 75.714844);
cairo_close_path(cr);
cairo_move_to(cr, 73.429688, 75.714844);
cairo_set_tolerance(cr, 0.1);
cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
cairo_matrix_init(&matrix, 1,0,0,1,0,-276.62701);
cairo_pattern_set_matrix(pattern, &matrix);
cairo_stroke_preserve(cr);
/********************/
cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
pattern = cairo_pattern_create_rgba(1,1,1,1);
cairo_set_source(cr, pattern);
cairo_pattern_destroy(pattern);
cairo_new_path(cr);
cairo_move_to(cr, 6.195313, 3.902344);
cairo_curve_to(cr, 5.050781, 3.902344, 4.171875, 4.648438, 3.835938, 5.292969);
cairo_curve_to(cr, 3.5, 5.941406, 3.5, 6.558594, 3.5, 6.558594);
cairo_line_to(cr, 3.496094, 6.574219);
cairo_line_to(cr, 3.496094, 71.671875);
cairo_curve_to(cr, 3.488281, 72.089844, 3.707031, 72.476563, 4.066406, 72.6875);
cairo_curve_to(cr, 4.429688, 72.898438, 4.875, 72.898438, 5.234375, 72.6875);
cairo_curve_to(cr, 5.59375, 72.476563, 5.8125, 72.089844, 5.808594, 71.671875);
cairo_line_to(cr, 5.808594, 6.628906);
cairo_curve_to(cr, 5.808594, 6.628906, 5.828125, 6.476563, 5.890625, 6.355469);
cairo_curve_to(cr, 5.949219, 6.238281, 5.839844, 6.214844, 6.195313, 6.214844);
cairo_line_to(cr, 72.242188, 6.214844);
cairo_curve_to(cr, 72.65625, 6.222656, 73.046875, 6.003906, 73.257813, 5.644531);
cairo_curve_to(cr, 73.46875, 5.28125, 73.46875, 4.835938, 73.257813, 4.476563);
cairo_curve_to(cr, 73.046875, 4.117188, 72.65625, 3.898438, 72.242188, 3.902344);
cairo_close_path(cr);
cairo_move_to(cr, 6.195313, 3.902344);
cairo_set_tolerance(cr, 0.1);
cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
cairo_set_fill_rule(cr, CAIRO_FILL_RULE_WINDING);
cairo_fill_preserve(cr);
 /********************/
}
