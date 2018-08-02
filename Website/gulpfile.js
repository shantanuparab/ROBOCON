// Gulpfile
var gulp = require('gulp');
var gutil = require('gulp-util');
var haml = require('gulp-ruby-haml');
var fileinclude = require('gulp-file-include');
var rename = require("gulp-rename");
var prettify = require('gulp-html-prettify');
var sass = require('gulp-sass');


gulp.task('scss', function () {
  gulp.src('src/scss/*.scss')
    .pipe(sass({style :'expanded'}))
      .on('error',   gutil.log)
    .pipe(gulp.dest('build/css/'));
});


gulp.task('haml', function () {
  gulp.src("src/haml/!(_)*.haml")
    .pipe(fileinclude({
    prefix: '@@',
    basepath: '@root',
  }))
  .pipe(haml().on('error', gutil.log))
  .pipe(prettify({indent_char: ' ', indent_size: 2}))
  .pipe(gulp.dest('build/html/'));
});

gulp.task('img', function () {
  gulp.src("src/img/*")
  .pipe(gulp.dest('build/img/'));
});

gulp.task('js', function () {
  gulp.src("src/js/*.js")
  .pipe(gulp.dest('build/js/'));
});


gulp.task('watch', function() {
  gulp.watch('src/haml/*.haml', ['haml']);
  gulp.watch('src/scss/*', ['scss']);
  gulp.watch('src/img/*', ['img']);
  gulp.watch('src/js/*.js', ['js']);

});

gulp.task('default', ['scss', 'haml', 'js', 'img', 'watch']);
