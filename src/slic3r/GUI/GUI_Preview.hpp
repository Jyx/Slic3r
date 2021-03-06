#ifndef slic3r_GUI_Preview_hpp_
#define slic3r_GUI_Preview_hpp_

#include <wx/panel.h>
#include "libslic3r/Point.hpp"

#include <string>

class wxNotebook;
class wxGLCanvas;
class wxBoxSizer;
class wxStaticText;
class wxChoice;
class wxComboCtrl;
class wxCheckBox;
class PrusaDoubleSlider;

namespace Slic3r {

class DynamicPrintConfig;
class Print;
class BackgroundSlicingProcess;
class GCodePreviewData;
class Model;

namespace GUI {

class GLCanvas3D;
class GLToolbar;

class View3D : public wxPanel
{
    wxGLCanvas* m_canvas_widget;
    GLCanvas3D* m_canvas;

#if !ENABLE_IMGUI
    wxPanel* m_gizmo_widget;
#endif // !ENABLE_IMGUI

    Model* m_model;
    DynamicPrintConfig* m_config;
    BackgroundSlicingProcess* m_process;

public:
    View3D(wxWindow* parent, Model* model, DynamicPrintConfig* config, BackgroundSlicingProcess* process);
    virtual ~View3D();

    wxGLCanvas* get_wxglcanvas() { return m_canvas_widget; }
    GLCanvas3D* get_canvas3d() { return m_canvas; }

    void set_view_toolbar(GLToolbar* toolbar);

    void set_as_dirty();
    void set_bed_shape(const Pointfs& shape);

    void select_view(const std::string& direction);
    void select_all();
    void delete_selected();
    void mirror_selection(Axis axis);

    void enable_toolbar_item(const std::string& name, bool enable);
    int check_volumes_outside_state() const;

    bool is_layers_editing_enabled() const;
    bool is_layers_editing_allowed() const;
    void enable_layers_editing(bool enable);

    bool is_dragging() const;
    bool is_reload_delayed() const;

    void reload_scene(bool refresh_immediately, bool force_full_scene_refresh = false);
    void render();

private:
    bool init(wxWindow* parent, Model* model, DynamicPrintConfig* config, BackgroundSlicingProcess* process);
};

class Preview : public wxPanel
{
    wxGLCanvas* m_canvas_widget;
    GLCanvas3D* m_canvas;
    wxBoxSizer* m_double_slider_sizer;
    wxStaticText* m_label_view_type;
    wxChoice* m_choice_view_type;
    wxStaticText* m_label_show_features;
    wxComboCtrl* m_combochecklist_features;
    wxCheckBox* m_checkbox_travel;
    wxCheckBox* m_checkbox_retractions;
    wxCheckBox* m_checkbox_unretractions;
    wxCheckBox* m_checkbox_shells;

    DynamicPrintConfig* m_config;
    BackgroundSlicingProcess* m_process;
    GCodePreviewData* m_gcode_preview_data;

    // Calling this function object forces Plater::schedule_background_process.
    std::function<void()> m_schedule_background_process;

    unsigned int m_number_extruders;
    std::string m_preferred_color_mode;

    bool m_loaded;
    bool m_enabled;

    PrusaDoubleSlider* m_slider {nullptr};

public:
    Preview(wxWindow* parent, DynamicPrintConfig* config, BackgroundSlicingProcess* process, GCodePreviewData* gcode_preview_data, std::function<void()> schedule_background_process = [](){});
    virtual ~Preview();

    wxGLCanvas* get_wxglcanvas() { return m_canvas_widget; }

    void set_view_toolbar(GLToolbar* toolbar);

    void set_number_extruders(unsigned int number_extruders);
    void set_canvas_as_dirty();
    void set_enabled(bool enabled);
    void set_bed_shape(const Pointfs& shape);
    void select_view(const std::string& direction);
    void set_viewport_from_scene(GLCanvas3D* canvas);
    void set_viewport_into_scene(GLCanvas3D* canvas);
    void set_drop_target(wxDropTarget* target);

    void load_print();
    void reload_print(bool force = false);
    void refresh_print();

private:
    bool init(wxWindow* parent, DynamicPrintConfig* config, BackgroundSlicingProcess* process, GCodePreviewData* gcode_preview_data);

    void bind_event_handlers();
    void unbind_event_handlers();

    void show_hide_ui_elements(const std::string& what);

    void reset_sliders();
    void update_sliders(const std::vector<double>& layers_z);

    void on_size(wxSizeEvent& evt);
    void on_choice_view_type(wxCommandEvent& evt);
    void on_combochecklist_features(wxCommandEvent& evt);
    void on_checkbox_travel(wxCommandEvent& evt);
    void on_checkbox_retractions(wxCommandEvent& evt);
    void on_checkbox_unretractions(wxCommandEvent& evt);
    void on_checkbox_shells(wxCommandEvent& evt);

    // Create/Update/Reset double slider on 3dPreview
    void create_double_slider();
    void update_double_slider(const std::vector<double>& layers_z, bool force_sliders_full_range = false);
    void fill_slider_values(std::vector<std::pair<int, double>> &values,
                            const std::vector<double> &layers_z);
    void reset_double_slider();
    // update DoubleSlider after keyDown in canvas
    void update_double_slider_from_canvas(wxKeyEvent& event);

    void load_print_as_fff();
    void load_print_as_sla();

    void on_sliders_scroll_changed(wxEvent& event);

};

} // namespace GUI
} // namespace Slic3r

#endif // slic3r_GUI_Preview_hpp_
