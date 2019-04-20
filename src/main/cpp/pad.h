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
cairo_move_to(cr, 73.472656, 75.761719);
cairo_curve_to(cr, 74.066406, 75.761719, 74.664063, 75.507813, 75.089844, 75.082031);
cairo_curve_to(cr, 75.511719, 74.65625, 75.765625, 74.0625, 75.765625, 73.464844);
cairo_line_to(cr, 75.765625, 3.511719);
cairo_curve_to(cr, 75.765625, 2.914063, 75.511719, 2.320313, 75.089844, 1.886719);
cairo_curve_to(cr, 74.664063, 1.460938, 74.070313, 1.21875, 73.472656, 1.21875);
cairo_line_to(cr, 3.507813, 1.21875);
cairo_curve_to(cr, 2.902344, 1.21875, 2.308594, 1.460938, 1.890625, 1.886719);
cairo_curve_to(cr, 1.460938, 2.320313, 1.214844, 2.914063, 1.214844, 3.511719);
cairo_line_to(cr, 1.214844, 73.464844);
cairo_curve_to(cr, 1.214844, 74.0625, 1.457031, 74.660156, 1.890625, 75.082031);
cairo_curve_to(cr, 2.308594, 75.507813, 2.902344, 75.761719, 3.507813, 75.761719);
cairo_close_path(cr);
cairo_move_to(cr, 73.472656, 75.761719);
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
cairo_move_to(cr, 73.472656, 75.761719);
cairo_curve_to(cr, 74.066406, 75.761719, 74.664063, 75.507813, 75.089844, 75.082031);
cairo_curve_to(cr, 75.511719, 74.65625, 75.765625, 74.0625, 75.765625, 73.464844);
cairo_line_to(cr, 75.765625, 3.511719);
cairo_curve_to(cr, 75.765625, 2.914063, 75.511719, 2.320313, 75.089844, 1.886719);
cairo_curve_to(cr, 74.664063, 1.460938, 74.070313, 1.21875, 73.472656, 1.21875);
cairo_line_to(cr, 3.507813, 1.21875);
cairo_curve_to(cr, 2.902344, 1.21875, 2.308594, 1.460938, 1.890625, 1.886719);
cairo_curve_to(cr, 1.460938, 2.320313, 1.214844, 2.914063, 1.214844, 3.511719);
cairo_line_to(cr, 1.214844, 73.464844);
cairo_curve_to(cr, 1.214844, 74.0625, 1.457031, 74.660156, 1.890625, 75.082031);
cairo_curve_to(cr, 2.308594, 75.507813, 2.902344, 75.761719, 3.507813, 75.761719);
cairo_close_path(cr);
cairo_move_to(cr, 73.472656, 75.761719);
cairo_set_tolerance(cr, 0.1);
cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
cairo_matrix_init(&matrix, 3.779528,0,0,3.779528,1634.586855,-171.179031);
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
cairo_move_to(cr, 4.65625, 71.714844);
cairo_line_to(cr, 4.65625, 6.601563);
cairo_curve_to(cr, 4.65625, 6.601563, 4.703125, 5.066406, 6.199219, 5.066406);
cairo_curve_to(cr, 8.890625, 5.066406, 72.28125, 5.066406, 72.28125, 5.066406);
cairo_set_tolerance(cr, 0.1);
cairo_set_antialias(cr, CAIRO_ANTIALIAS_DEFAULT);
cairo_matrix_init(&matrix, 3.779528,0,0,3.779528,1634.586855,-171.179031);
cairo_pattern_set_matrix(pattern, &matrix);
cairo_stroke_preserve(cr);
/********************/
}
