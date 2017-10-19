import sys

import driver

def get_II(bb):
    # TODO
    return "3"

class SwplMetricWorker(driver.MetricWorker):
    def match_p(self, target_config, test_name):
        if target_config.compiler == "ClangLLVM":
            return True
        return False

    def set_up_before_getting_data(self, target_config, db_filename, bb_list, column_list):
        pass

    def get_column_name_list(self):
        return ["II"]

    def get_data_list(self, target_config, bb):
        ii = get_II(bb)
        return [ii]

    def get_summary_list(self, target_config):
        return ["-"]

if __name__ == "__main__":
    swpl_metric_worker = SwplMetricWorker()
    driver.run_metric_work_if_match_p(sys.argv, swpl_metric_worker)
