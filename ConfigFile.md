# skylium.cfg file description #

There are several settings for Skylium that are read during Skylium::init(). The config file should be placed in a main directory (where the main.cpp is located) and named **skylium.cfg**. The possible variables are:
<ul>
<li> <b>tellmeabout</b> - configures the output verbosity. Possible settings are: everything, constructors, all_constructors, destructors, params, all_params, shaders, buffer, warnings, errors. Default:<br>
<pre><code>tellmeabout warnings,errors<br>
</code></pre>
</li>
<li> <b>fullscreen</b> - there's probably nothing to explain. True/false. Default:<br>
<pre><code>fullscreen false<br>
</code></pre>
</li>
<li> <b>hud_exists</b> - if true, hud exists. If false, hud still exists, but can't be showed - to be fixed soon. Default:<br>
<pre><code>hud_exists true<br>
</code></pre>
</li>
<li> <b>mouse_visible</b> - if true, the mouse pointer will be showed - useful in debugging. Default:<br>
<pre><code>mouse_visible true<br>
</code></pre>
</li>
<li> <b>gl_size</b> - minimum size of each buffer for the Skylium to be started. If the graphics card does not support that many bytes, Skylium won't start. Otherwise, OpenGL will use as many bytes as the graphics card supports.<br>
<pre><code>gl_red_size 8<br>
gl_green_size 8<br>
gl_blue_size 8<br>
gl_depth_size 24<br>
</code></pre>
</li>
<li> <b>create_mipmaps</b> - if true, Skylium will generate mipmaps.<br>
<pre><code>create_mipmaps true<br>
</code></pre>
</li>
</ul>