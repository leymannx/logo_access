<?php

/**
 * @file
 * Admin settings for Logo Access.
 */

/**
 * Logo Access settings form.
 */
function logo_access_settings() {
  $default_theme = variable_get('theme_default', NULL);
  $current_logo_path = theme_get_setting('logo_path', $default_theme);
  $current_logo_src = file_create_url($current_logo_path);
  $current_logo_fid = variable_get('logo_access');
  
  $form['default_theme'] = array(
    '#type' => 'hidden',
    '#value' => $default_theme,
  );
  $form['current_logo'] = array(
    '#prefix' => '<div class="form-item"><label>'. t('Current logo') . '</label>',
    '#markup' => '<img src="' . $current_logo_src . '" />',
    '#suffix' => '</div>',
  );
  if (module_exists('media')) {
    $form['logo_access'] = array(
      '#type'           => 'media',
      '#tree'           => TRUE,
      '#title'          => t('Choose a file'),
      '#input'          => TRUE,
      '#default_value'  => $current_logo_fid,
      '#media_options'  => array(
        'global' => array(
          'types' => array('image'),
          'file_extensions' => 'jpeg jpg png gif',
          'file_directory' => 'logo',
        ),
      ),
    );
  } else {
    $form['logo_access'] = array(
      '#type'               => 'managed_file',
      '#title'              => t('Choose a file'),
      '#default_value'      => $current_logo_fid,
      '#upload_location'    => 'public://logo',
      '#upload_validators'  => array(
        'file_validate_extensions' => array('gif png jpg jpeg'),
      ),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Logo Access form submit handler.
 */
function logo_access_settings_submit($form, &$form_state) {
  $new_logo_fid = $form_state['values']['logo_access'];
  $file = file_load($new_logo_fid);
  $new_logo_path = $file->uri;

  $default_theme = $form_state['values']['default_theme'];
  $values = variable_get('theme_' . $default_theme . '_settings');
  $values['logo_path'] = $new_logo_path;

  variable_set('logo_access', $new_logo_fid);
  variable_set('theme_' . $default_theme . '_settings', $values);
}
