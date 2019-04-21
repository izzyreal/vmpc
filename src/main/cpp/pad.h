int cairo_code_pad_get_width() { return 77; }
int cairo_code_pad_get_height() { return 77; }
void cairo_code_pad_render(cairo_t *cr) {
cairo_surface_t *temp_surface;
cairo_t *old_cr;
cairo_pattern_t *pattern;
cairo_matrix_t matrix;

cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
pattern = cairo_pattern_create_rgba(0.439216,0.439216,0.439216,1);
cairo_set_source(cr, pattern);
cairo_pattern_destroy(pattern);
cairo_new_path(cr);
cairo_move_to(cr, 73.429688, 75.714844);
cairo_curve_to(cr, 74.027344, 75.714844, 74.621094, 75.460938, 75.046875, 75.039063);
cairo_curve_to(cr, 75.46875, 74.613281, 75.722656, 74.019531, 75.722656, 73.421875);
cairo_line_to(cr, 75.722656, 3.507813);
cairo_curve_to(cr, 75.722656, 2.910156, 75.46875, 2.316406, 75.046875, 1.882813);
cairo_curve_to(cr, 74.621094, 1.457031, 74.027344, 1.214844, 73.429688, 1.214844);
cairo_line_to(cr, 3.507813, 1.214844);
cairo_curve_to(cr, 2.902344, 1.214844, 2.304688, 1.457031, 1.890625, 1.882813);
cairo_curve_to(cr, 1.457031, 2.316406, 1.214844, 2.910156, 1.214844, 3.507813);
cairo_line_to(cr, 1.214844, 73.421875);
cairo_curve_to(cr, 1.214844, 74.019531, 1.457031, 74.613281, 1.890625, 75.039063);
cairo_curve_to(cr, 2.304688, 75.460938, 2.902344, 75.714844, 3.507813, 75.714844);
cairo_close_path(cr);
cairo_move_to(cr, 73.429688, 75.714844);
cairo_set_tolerance(cr, 0.1);
cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
cairo_set_fill_rule(cr, CAIRO_FILL_RULE_WINDING);
cairo_fill_preserve(cr);
 /********************/
cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
cairo_set_line_width(cr, 0.64229);
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
cairo_line_to(cr, 3.507813, 1.214844);
cairo_curve_to(cr, 2.902344, 1.214844, 2.304688, 1.457031, 1.890625, 1.882813);
cairo_curve_to(cr, 1.457031, 2.316406, 1.214844, 2.910156, 1.214844, 3.507813);
cairo_line_to(cr, 1.214844, 73.421875);
cairo_curve_to(cr, 1.214844, 74.019531, 1.457031, 74.613281, 1.890625, 75.039063);
cairo_curve_to(cr, 2.304688, 75.460938, 2.902344, 75.714844, 3.507813, 75.714844);
cairo_close_path(cr);
cairo_move_to(cr, 73.429688, 75.714844);
cairo_set_tolerance(cr, 0.1);
cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
cairo_matrix_init(&matrix, 3.77742,0,0,3.77742,1633.674246,-171.086333);
cairo_pattern_set_matrix(pattern, &matrix);
cairo_stroke_preserve(cr);
/********************/
cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
cairo_set_line_width(cr, 0.611705);
cairo_set_miter_limit(cr, 4);
cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER);
pattern = cairo_pattern_create_rgba(1,1,1,1);
cairo_set_source(cr, pattern);
cairo_pattern_destroy(pattern);
cairo_new_path(cr);
cairo_move_to(cr, 4.652344, 71.671875);
cairo_line_to(cr, 4.652344, 6.59375);
cairo_curve_to(cr, 4.652344, 6.59375, 4.699219, 5.058594, 6.195313, 5.058594);
cairo_curve_to(cr, 8.886719, 5.058594, 72.238281, 5.058594, 72.238281, 5.058594);
cairo_set_tolerance(cr, 0.1);
cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
cairo_matrix_init(&matrix, 3.77742,0,0,3.77742,1633.674246,-171.086333);
cairo_pattern_set_matrix(pattern, &matrix);
cairo_stroke_preserve(cr);
/********************/
}
